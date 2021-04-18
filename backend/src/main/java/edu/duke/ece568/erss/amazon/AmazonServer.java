package edu.duke.ece568.erss.amazon;

import edu.duke.ece568.erss.amazon.protos.WorldAmazon.*;
import edu.duke.ece568.erss.amazon.QueryFunctions;
import edu.duke.ece568.erss.amazon.protos.AmazonUps.*;
import org.checkerframework.checker.units.qual.A;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.lang.reflect.AccessibleObject;
import java.io.InputStreamReader;
import com.google.protobuf.CodedInputStream;
import com.google.protobuf.CodedOutputStream;
import com.google.protobuf.GeneratedMessageV3;
import java.net.Socket;
import java.net.SocketException;
import java.net.ServerSocket;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
public class AmazonServer {

    public static final String WORLD_HOST_IP = "vcm-18228.vm.duke.edu";
    public static final int AMAZON_PORTNUM = 23456;

    public static final int UPS_PORTNUM = 34567;

    public static final int TIME_RESEND = 5000;

    // The list of all warehouses.
    private final List<AInitWarehouse> warehouses;
    // The InputStream for Amazon_World Socket, Amazon receive msg from this.
    private InputStream input;
    // The OurputStream for Amazon_World Socket, Amazon Send msg by this.
    private OutputStream output;
    // The Socket for UPS.
    Socket UPSocket;
    // Sequence Number
    private long globalSeqNum;

    // ThreadPool to execute the thread.
    private final ThreadPoolExecutor threadPool;

    // This will help us find the package.
    private Map<Long, Package> packageMap; 

    private Map<Long, Timer> worldRequestMap;
    
    private Map<Long, Timer> UPSRequestMap;

    private FrontEndListener frontEndListener;

    public AmazonServer(){
        //warehouses = new ArrayList<>();
        this.warehouses = QueryFunctions.qWarehouses();
        this.threadPool = new ThreadPoolExecutor(20, 80, 5, TimeUnit.SECONDS, new LinkedBlockingQueue<Runnable>());
        this.globalSeqNum = 0;
    }

    public <T extends GeneratedMessageV3.Builder<?>> boolean sendMSG(T builder, OutputStream output) throws IOException{
            CodedOutputStream codedOutputStream = CodedOutputStream.newInstance(output);
            codedOutputStream.writeUInt32NoTag(builder.build().toByteArray().length);
            builder.build().writeTo(codedOutputStream);
            codedOutputStream.flush();
            return true;
    }

    void runFrontEndListener() {
        frontEndListener = new FrontEndListener(packageID -> {
            System.out.println(String.format("Receive new buying request, id: %d", packageID));
            // use purchase (packageID) method in amazonserver
        });
        frontEndListener.start();
    }

    // public <T extends GeneratedMessageV3.Builder<?>> boolean recvMSG(InputStream input){
    //     CodedInputStream codedInputStream = CodedInputStream.newInstance(UPSocket.getInputStream());
    //     createWorld cWorld = createWorld.parseFrom(codedInputStream.readByteArray());
    // }

    public void initiaition() throws IOException{
        System.out.println("Initializing the AmazonServer.");
        System.out.println("UPS PORT is : " + UPS_PORTNUM);

        while(true){
            try{
                waitingForUPS();
            }catch(Exception e){
                System.out.println("Socket Error!");
                System.err.println(e.toString());
            }
            if(UPSocket != null){
                //createWorld.Builder cWorld = createWorld.newBuilder();
                //cWorld.mergeFrom(UPSocket.getInputStream());
                
                System.out.println("Socket Success!");
                // createWorld cWorld = createWorld.parseDelimitedFrom(UPSocket.getInputStream());
                CodedInputStream codedInputStream = CodedInputStream.newInstance(UPSocket.getInputStream());
                createWorld cWorld = createWorld.parseFrom(codedInputStream.readByteArray());
                System.out.println("cWorld");
                System.out.println(cWorld);
                if(cWorld.hasWorldID()){
                    try{
                        System.out.println("begin world");
                        boolean connected = amazonToWorldConnection(cWorld.getWorldID());
                        if(connected == true){
                            System.out.println("Connected to the World.");
                            
                            ACommand.Builder builder = ACommand.newBuilder();
                            
                            builder.addAcks(cWorld.getSeqnum());
                            builder.setIsRequest(false);
                            
                            sendMSG(builder, this.output);
                            // CodedOutputStream codedOutputStream = CodedOutputStream.newInstance(output);
                            // codedOutputStream.writeUInt32NoTag(builder.build().toByteArray().length);
                            // builder.build().writeTo(codedOutputStream);
                            // codedOutputStream.flush();
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
        System.out.println(builder);

        //AConnected.Builder builderAConnected = AConnected.newBuilder();
        // Send Msg to the World or warehouses
        sendMSG(builder, output);
        CodedInputStream codedInputStream = CodedInputStream.newInstance(input);
        AConnected acc = AConnected.parseFrom(codedInputStream.readByteArray());
        //builderAConnected.mergeFrom(input);
        //AConnected acc = AConnected.parseFrom(input);

        System.out.println("WorldID: " + acc.getWorldid());
        System.out.println("Result: " + acc.getResult());
        
        return acc.getResult().equals("connected!");
    }

    /**
     * The proto says: every World Response will have 6 fields to handle
     * and it will n
     * @return
     */
    public boolean worldResponseHandler(AResponses res) throws IOException{
        System.out.println("Receive World Response:" + res.toString());
        worldAckSender(res);
        for(APurchaseMore apc : res.getArrivedList()){
            //TODO : Purchase More Function
        }
        for(APacked ap : res.getReadyList()){
            //TODO : Pack Package Function
        }
        for(ALoaded al : res.getLoadedList()){
            //TODO : Load Function
        }
        for(AErr ae : res.getErrorList()){
            System.err.println(ae.getErr());
        }
        for(APackage ap : res.getPackagestatusList()){
            System.out.println(ap.getStatus());

        }
        return true;
    }

    public boolean UPSResponseHandler(UCommand res){
        System.out.println("Receive UPS Response:" + res.toString());
        //TODO: replace placeholder with useful code
        boolean placeHolder = true;
        return placeHolder;
    }

    public long seqNumGenerator(){
        long temp = globalSeqNum;
        globalSeqNum += 1;
        return temp;
    }

    public void packPackage(long packageId){
        if(!packageMap.containsKey(packageId)){
            System.out.println("There is no corresponding Package.");
            return;
        }
        System.out.println("Packing " + packageId);
        Package pac = packageMap.get(packageId);
        pac.setStatus(Package.PACKING);
        threadPool.execute(()->{
            ACommands.Builder pack = ACommands.newBuilder();
            long seq = seqNumGenerator();
            APack toPack = packageMap.get(packageId).getPack();
            pack.addTopack(toPack.toBuilder().setSeqnum(seq).build());
            commandToWorld(seq, pack);
        });
    }

    public warehouse getWarehouse(int warehouseNum){
        for(AInitWarehouse a : warehouses){
            if(a.getId() == warehouseNum){
                warehouse.Builder wb = warehouse.newBuilder();
                wb.setWarehouseID(warehouseNum);
                wb.setX(a.getX());
                wb.setY(a.getY());
                return wb.build();
            }
        }
        return null;
    }

    public newShipment getNewShipment(AProduct p, warehouse w, Package pac, long seqNum, long shipId){
        newShipment.Builder newShip = newShipment.newBuilder();
        newShip.setShipID(shipId);
        product.Builder pBuilder = product.newBuilder();
        pBuilder.setProductID(p.getId());
        pBuilder.setDescription(p.getDescription());
        pBuilder.setCount(p.getCount());
        newShip.setProductInfo(pBuilder.build());
        newShip.setDestinationInfo(pac.getDestination());
        newShip.setWarehouseInfo(w);
        newShip.setSeqnum(seqNum);
        return newShip.build();
    }
    
    public void pickPackage(long packageId) {
        if(!packageMap.containsKey(packageId)){
            return;
        }
        System.out.println("Tell Truck to pick " + packageId);
        Package pac = packageMap.get(packageId);
        threadPool.execute(()->{
            ACommand.Builder toUps = ACommand.newBuilder();
            toUps.setIsRequest(false);
            long seqNum = seqNumGenerator();
            long shipId = pac.getShipID();
            APack apc = pac.getPack();
            warehouse w = getWarehouse(pac.getwarehouseID());
            for(AProduct p : apc.getThingsList()){
                newShipment n = getNewShipment(p, w, pac, seqNum, shipId);
                toUps.addNewShipmentCreated(n);
            }
            // Send msg to UPS
            commandToUPS(seqNum, toUps);
        });
    }
    
    public void commandToWorld(long seq, ACommands.Builder command){
        System.out.println("Sending Command to World");
        command.setSimspeed(100);
        Timer work = new Timer();
        work.schedule(new TimerTask(){
            @Override
            public void run(){
                synchronized(output){
                    try {
                        sendMSG(command, output);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }, 0, TIME_RESEND);
        worldRequestMap.put(seq, work);

    }
    
    public void commandToUPS(long seq, ACommand.Builder command) {
        System.out.println("Sending Command to UPS");

        Timer work = new Timer();
        work.schedule(new TimerTask(){
            @Override
            public void run(){
                OutputStream out = null;
                try {
                    out = UPSocket.getOutputStream();
                } catch (IOException e) {
                    e.printStackTrace();
                }
                synchronized(out){
                    try {
                        sendMSG(command, out);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }, 0, TIME_RESEND);
        UPSRequestMap.put(seq, work);
    }
    
    // public void buyOrder(long packageId){
    //     System.out.println("Buying " + packageId);
    //     this.threadPool.execute(()->{
    //         long seqnum = seqNumGenerator();
    //         APurchaseMore.Builder 
    //     });
    // }


    public void worldAckSender(AResponses res) throws IOException{
        ArrayList<Long> seq = new ArrayList<>();
        for(APurchaseMore apc : res.getArrivedList()){
            seq.add(apc.getSeqnum());
        }
        for(APacked ap : res.getReadyList()){
            seq.add(ap.getSeqnum());
        }
        for(ALoaded al : res.getLoadedList()){
            seq.add(al.getSeqnum());
        }
        for(AErr ae : res.getErrorList()){
            seq.add(ae.getSeqnum());
        }
        for(APackage ap : res.getPackagestatusList()){
            seq.add(ap.getSeqnum());
        }
        // This means it at least needs to send one ack.
        if(seq.size() > 0){
            ACommand.Builder ares = ACommand.newBuilder();
            for(long s : seq){
                ares.addAcks(s);
            }
            synchronized(output){
                System.out.println("Now Send Acks to the World.");
                sendMSG(ares, output);    
            }
        }
    }
}
