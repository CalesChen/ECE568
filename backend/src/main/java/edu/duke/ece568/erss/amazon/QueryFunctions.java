package edu.duke.ece568.erss.amazon;

import java.lang.Thread.State;
import java.sql.*;
import java.util.*;

import edu.duke.ece568.erss.amazon.Address;
import edu.duke.ece568.erss.amazon.protos.WorldAmazon.AInitWarehouse;
import edu.duke.ece568.erss.amazon.protos.WorldAmazon.APurchaseMore;

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

    public static APurchaseMore.Builder qPackage(long packageId){
        try{
            Class.forName("org.postgresql.Driver");
            Connection C = DriverManager.getConnection(dbURL, dbUSER, dbPASSWD);

            Statement work = C.createStatement();
            String sql = String.format("update %s set status = %s where id = %d", 
                        TABLE_PACKAGE, status, packageId);
            work.executeUpdate(sql);
            C.commit();
            work.close();
            C.close();
        }catch(Exception e){
            System.err.println(e.toString());
        }
        return null;
    }

    public static Address qAddress(long packageId){
        
    }
}
