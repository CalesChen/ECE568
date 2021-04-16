package edu.duke.ece568.erss.amazon;

import edu.duke.ece568.erss.amazon.protos.WorldAmazon.*;
import edu.duke.ece568.erss.amazon.protos.AmazonUps.*;
import org.checkerframework.checker.units.qual.A;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.io.InputStreamReader;
import java.net.Socket;
import java.net.SocketException;
import java.net.ServerSocket;
import java.util.*;
public class AmazonServer {

    public static final String WORLD_HOST_IP = "568multicore.colab.duke.edu";
    public static final int AMAZON_PORTNUM = 23456;

    public static final int UPS_PORTNUM = 8888;

    private final List<AInitWarehouse> warehouses;

    private InputStream input;
    private OutputStream output;
    Socket UPSocket;

    public AmazonServer(){
        //warehouses = new ArrayList<>();
        warehouses = QueryFunctions.qWarehouses();
        //waitingForUPS();
    }

    public void initiaition() throws IOException{
        System.out.println("Initializing the AmazonServer.");
        System.out.println("UPS PORT is : " + UPS_PORTNUM);

        while(true){
            try{
                waitingForUPS();
            }catch(Exception e){
                System.err.println(e.toString());
            }
            if(UPSocket != null){
                createWorld.Builder cWorld = createWorld.newBuilder();
                cWorld.mergeFrom(UPSocket.getInputStream());
                if(cWorld.hasWorldID()){
                    try{
                        boolean connected = amazonToWorldConnection(cWorld.getWorldID());
                        if(connected == true){
                            System.out.println("Connected to the World.");
                            break;
                        }
                    }catch(Exception e){
                        System.err.println(e.toString());
                    }
                    
                }
                  
            }
            
        }
    }

    public void waitingForUPS() throws IOException, SocketException{
        ServerSocket ssockt = new ServerSocket(UPS_PORTNUM);
        // Will wait for 10s without throw exception.
        ssockt.setSoTimeout(10000);
        try{
            System.out.println("Waiting for UPS");
            UPSocket = ssockt.accept();
        }catch(Exception e){
            System.out.println(e.getMessage());
        }
        ssockt.close();
        //TODO: The ssockt needs to be closed.
        // this.input = UPSocket.getInputStream();
        // BufferedReader reader = new BufferedReader(new InputStreamReader(this.input));
        // String req = reader.readLine();

        // System.out.println(req);

        // this.output = UPSocket.getOutputStream();
        // PrintWriter writer = new PrintWriter(this.output);
        // writer.write("received " + req);
        // writer.flush();
        // UPSocket.close();
    }

   
    public boolean amazonToWorldConnection(long worldId) throws IOException {
        Socket socket = new Socket(WORLD_HOST_IP, AMAZON_PORTNUM);
        input = socket.getInputStream();
        output = socket.getOutputStream();
        AConnect.Builder builder = AConnect.newBuilder();
        builder.setIsAmazon(true);
        builder.addAllInitwh(warehouses);
        builder.setWorldid(worldId);
    
        AConnected.Builder builderAConnected = AConnected.newBuilder();
        // Send Msg to the World or warehouses
        builder.build().writeTo(output);
        builderAConnected.mergeFrom(input);

        System.out.println("WorldID: " + builderAConnected.getWorldid());
        System.out.println("Result: " + builderAConnected.getResult());
        
        return builderAConnected.getResult().equals("connected!");
    }

    /**
     * The proto says: every World Response will have 6 fields to handle
     * and it will n
     * @return
     */
    public boolean worldResponseHandler(){
        return true;
    }
}
