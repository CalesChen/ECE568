package edu.duke.ece568.erss.amazon;

import java.lang.Thread.State;
import java.sql.*;
import java.util.*;

import edu.duke.ece568.erss.amazon.Address;
import edu.duke.ece568.erss.amazon.protos.WorldAmazon;
import edu.duke.ece568.erss.amazon.protos.WorldAmazon.AInitWarehouse;
import edu.duke.ece568.erss.amazon.protos.WorldAmazon.APurchaseMore;
import edu.duke.ece568.erss.amazon.protos.WorldAmazon.AProduct;
import org.checkerframework.checker.units.qual.A;

public class QueryFunctions {
    private static final String TABLE_WAREHOUSE = "\"amazonWeb_warehouse\"";
    private static final String TABLE_ORDER = "\"amazonWeb_order\"";
    private static final String TABLE_PACKAGE = "\"amazonWeb_package\"";
    private static final String TABLE_CATEGORY = "\"amazonWeb_category\"";
    private static final String TABLE_PRODUCT = "\"amazonWeb_product\"";

    private static final String dbURL = "jdbc:postgresql://localhost/AMAZON";
    private static final String dbUSER = "postgres";
    private static final String dbPASSWD = "passw0rd";

    public static List<AInitWarehouse> qWarehouses(){
        List<AInitWarehouse> ret = new ArrayList<>();
        try{
            Class.forName("org.postgresql.Driver");
            Connection C = DriverManager.getConnection(dbURL, dbUSER, dbPASSWD);

            Statement work = C.createStatement();
            String sql = String.format("select * from %s", TABLE_WAREHOUSE);
            ResultSet result = work.executeQuery(sql);
            
            while(result.next()){
                AInitWarehouse.Builder builder = AInitWarehouse.newBuilder();
                builder.setId(result.getInt("id"));
                builder.setX(result.getInt("address_x"));
                builder.setY(result.getInt("address_y"));
                ret.add(builder.build());
            }
            work.close();
            C.close();
            return ret;
        }catch(ClassNotFoundException | SQLException e){
            System.err.println(e.toString());
        }
        return ret;
    }

    public static boolean updateStatus(long packageId, String status){
        try{
            Class.forName("org.postgresql.Driver");
            Connection C = DriverManager.getConnection(dbURL, dbUSER, dbPASSWD);

            Statement work = C.createStatement();
            String sql = String.format("update %s set status = %s where id = %d", TABLE_PACKAGE, status, packageId);
            work.executeUpdate(sql);
            C.commit();
            work.close();
            C.close();
            return true;
        }catch(Exception e){
            System.err.println(e.toString());
        }
        return false;
    }
    public static boolean updateStatus(long packageId, int status){
        try{
            Class.forName("org.postgresql.Driver");
            Connection C = DriverManager.getConnection(dbURL, dbUSER, dbPASSWD);

            Statement work = C.createStatement();
            String s;
            if(status == 0){
                s = Package.FINISHED;
            }else{
                s = "Error";
            }
            String sql = String.format("update %s set status = %s where id = %d", TABLE_PACKAGE, s, packageId);
            work.executeUpdate(sql);
            C.commit();
            work.close();
            C.close();
            return true;
        }catch(Exception e){
            System.err.println(e.toString());
        }
        return false;
    }

    public static APurchaseMore.Builder qPackage(long packageId){
        try{
            Class.forName("org.postgresql.Driver");
            Connection C = DriverManager.getConnection(dbURL, dbUSER, dbPASSWD);
            //need product id, description, count to construct Aproduct
            //need whnum, product list, seqnum to construct Apurchasemore
            Statement work = C.createStatement();
            String sql = String.format("SELECT products.id, products.description, orders.product_num, packages.warehouse_id " +
                            "FROM %s AS products, %s AS orders, %s AS packages " +
                            "WHERE products.id=orders.product_id AND orders.package_id = %d AND packages.id = %d;",
                    TABLE_PRODUCT, TABLE_ORDER, TABLE_PACKAGE, packageId, packageId);
            ResultSet result = work.executeQuery(sql);
            APurchaseMore.Builder purchaseBuilder = APurchaseMore.newBuilder();
            while (result.next()){
                AProduct.Builder productBuilder = AProduct.newBuilder();
                productBuilder.setId(result.getInt("id"));
                productBuilder.setCount(result.getInt("product_num"));
                productBuilder.setDescription(result.getString("description"));
                purchaseBuilder.setWhnum(result.getInt("warehouse_id"));
                purchaseBuilder.addThings(productBuilder.build());
            }
            work.close();
            C.close();
            return purchaseBuilder;
        }catch(Exception e){
            System.err.println(e.toString());
        }
        return null;
    }

    public static Address qAddress(long packageId){
        try{
            Class.forName("org.postgresql.Driver");
            Connection C = DriverManager.getConnection(dbURL, dbUSER, dbPASSWD);
            Statement work = C.createStatement();
            String sql = String.format("select address_x, address_y from %s where id = %d", TABLE_PACKAGE, packageId);
            ResultSet result = work.executeQuery(sql);
            Address dest = null;
            while(result.next()){
                int x = result.getInt("address_x");
                int y = result.getInt("address_y");
                dest = new Address(x,y);
            }
            work.close();
            C.close();
            return dest;
        }catch(ClassNotFoundException | SQLException e){
            System.err.println(e.toString());
        }
        return null;
    }

    public static boolean updtaeTrackingNum(long packageId, long trackingNum){
        try{
            Class.forName("org.postgresql.Driver");
            Connection C = DriverManager.getConnection(dbURL, dbUSER, dbPASSWD);
            Statement work = C.createStatement();
            String sql = String.format("update %s set track_num = %d where id = %d", TABLE_PACKAGE, trackingNum, packageId);
            work.executeUpdate(sql);
            C.commit();
            work.close();
            C.close();
            return true;
        }catch(Exception e){
            System.err.println(e.toString());
        }
        return false;
    }
}
