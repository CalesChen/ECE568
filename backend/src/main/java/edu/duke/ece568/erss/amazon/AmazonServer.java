package edu.duke.ece568.erss.amazon;

import edu.duke.ece568.erss.amazon.protos.WorldAmazon.*;

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

    //private final ArrayList<AInitWareHouse> warehouses;

    private InputStream input;
    private OutputStream output;
    Socket UPSocket;

    public AmazonServer(){
        //waitingForUPS();
    }

    public void waitingForUPS() throws IOException, SocketException{
        ServerSocket ssockt = new ServerSocket(UPS_PORTNUM);
        // Will wait for 10s without throw exception.
        ssockt.setSoTimeout(10000);
        try{
            UPSocket = ssockt.accept();
        }catch(Exception e){
            System.out.println(e.getMessage());
        }
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

   
    public boolean amazonToWorldConnection(long worldId){
        Socket socket = new Socket(WORLD_HOST_IP, AMAZON_PORTNUM);
        input = socket.getInputStream();
        output = socket.getOutputStream();
        AConnectOrBuilder builder = AConnect.newBuilder();
        builder.setIsAmazon(true);
        builder.addAllInitwh(warehouses);
        builder.setWorldid(worldId);
    
        AConnectedOrBuilder builderAConnected = AConnected.newBuilder();
        // Send Msg to the World or warehouses
        builder.build().writeTo(output);
        builderAConnected.parseFrom(input);

        System.out.println("WorldID: " + builderAConnected.getWorldid());
        System.out.println("Result: " + builderAConnected.getResult());
        
        return builderAConnected.getResult().equals("connected!");
    }
}
