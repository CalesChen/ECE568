package edu.duke.ece568.erss.amazon;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class FrontEndListener extends Thread{
    OrderListener orderListener;

    public FrontEndListener(OrderListener orderListener){
        this.orderListener = orderListener;
        this.setDaemon(true);
    }
    @Override
    public void run(){
        while (!Thread.currentThread().isInterrupted()){
            try (ServerSocket daemonServer = new ServerSocket(8888)){
                System.out.println("Listening connection from front-end at 8888");
                while (!Thread.currentThread().isInterrupted()){
                    Socket s = daemonServer.accept();
                    if (s != null){
                        processPackage(s);
                    }
                }
            }catch (Exception e){
                System.err.println(e.toString());
            }
        }
    }

    public void processPackage(Socket s) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(s.getInputStream()));
        PrintWriter writer = new PrintWriter(s.getOutputStream());
        String req = reader.readLine();
        System.out.println("new buying request: " + req);
        long id = Long.parseLong(req);
        writer.write(String.format("received %d", id));
        writer.flush();
        // close the connection
        s.close();
        if (orderListener != null){
            orderListener.onOrder(id);
        }
    }
}
