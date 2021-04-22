package edu.duke.ece568.erss.amazon;

import com.google.protobuf.InvalidProtocolBufferException;
import edu.duke.ece568.erss.amazon.protos.WorldAmazon.*;
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

    public static final String WORLD_HOST_IP = "vcm-19389.vm.duke.edu";
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
        worldRequestMap = new ConcurrentHashMap<>();
        UPSRequestMap = new ConcurrentHashMap<>();
        packageMap = new ConcurrentHashMap<>();
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
            buyOrder(packageID);
        });
        frontEndListener.start();
    }
    public void runWorldServer(){
        Thread worldServer = new Thread(()->{
            while(!Thread.currentThread().isInterrupted()){
                try{
                    CodedInputStream codedInputStream = CodedInputStream.newInstance(this.input);
                    AResponses resp = AResponses.parseFrom(codedInputStream.readByteArray());
                    worldResponseHandler(resp);
                } catch (InvalidProtocolBufferException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });
        worldServer.start();
    }
    public void runUPServer(){
        Thread UPSServer = new Thread(()->{
            while(!Thread.currentThread().isInterrupted()){
                if(UPSocket != null){
                    try{
                        
                        InputStream UpsInput = UPSocket.getInputStream();
                        if(UpsInput.available() == 0){
                            continue;
                        }
                        CodedInputStream codedInputStream = CodedInputStream.newInstance(UpsInput);
                        UCommand resp = UCommand.parseFrom(codedInputStream.readByteArray());
                        UPSResponseHandler(resp);
                    } catch (InvalidProtocolBufferException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        UPSServer.start();
    }

    public void runAll(){
        threadPool.prestartAllCoreThreads();
        runUPServer();
        runFrontEndListener();
        runWorldServer();

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
                
                ACommand.Builder ab = ACommand.newBuilder();
                //ab.setIsRequest(false);
                ab.addAcks(cWorld.getSeqnum());
                long seq = seqNumGenerator();
                sendMSG(ab, UPSocket.getOutputStream());

                System.out.println("cWorld");
                System.out.println(cWorld);
                if(cWorld.hasWorldID()){
                    try{
                        System.out.println("begin world");
                        boolean connected = amazonToWorldConnection(cWorld.getWorldID());
                        if(connected == true){
                            System.out.println("Connected to the World.");
                            
                            ACommands.Builder builder = ACommands.newBuilder();
                            
                            builder.addAcks(cWorld.getSeqnum());
                            //builder.setIsRequest(false);
                            
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
            purchaseFinished(apc);
        }
        for(APacked ap : res.getReadyList()){
            //TODO : Pack Package Function
            toLoadPackage(ap.getShipid());
        }
        List<Long> loadedIds = new ArrayList<>();
        for(ALoaded al : res.getLoadedList()){
            //TODO : Load Function
            loadedIds.add(al.getShipid());
        }
        if(loadedIds.size() > 0){
            toDelieverPackage(loadedIds);
        }
        for(AErr ae : res.getErrorList()){
            System.err.println(ae.getErr());
        }
        for(APackage ap : res.getPackagestatusList()){
            System.out.println(ap.getStatus());
        }
        for(long ack : res.getAcksList()){
            if(worldRequestMap.containsKey(ack)){
                worldRequestMap.get(ack).cancel();
                worldRequestMap.remove(ack);
            }
        }
        if(res.hasFinished()){
            System.out.println("Amazon Disconnected from the World!");
        }
        return true;
    }

    public boolean UPSResponseHandler(UCommand res) throws IOException{
        System.out.println("Receive UPS Response:" + res.toString());

        UPSAckSender(res);
        //TODO: replace placeholder with useful code
        for(long ack : res.getAcksList()){
            System.out.println("UPS ACK is :" + ack);
            if(UPSRequestMap.containsKey(ack)){
                
                UPSRequestMap.get(ack).cancel();
                UPSRequestMap.remove(ack);
                System.out.println("Canceled UPS ACK is :" + ack);
            }
        }
        for(newShipmentResponse nsp : res.getTrackingNumberCreatedList()){
            QueryFunctions.updateStatus(nsp.getShipID(), nsp.getStatus());
            QueryFunctions.updtaeTrackingNum(nsp.getShipID(), nsp.getTrackingNumber());
        }
        for(truckArrival ta : res.getArrivedList()){
            truckLoader(ta.getShipIDList(), ta.getTruckID());
        }
        for(deliveredPackage dp : res.getDeliveredList()){
            // TODO : Process the Delievered Package.
            System.out.println("Package Delievered.\n");
            long packageId = QueryFunctions.qPackageId(dp.getTrackingNumber());
            finishDeliver(packageId);
        }
        for(checkUsernameResponse cur : res.getCheckUserList()){
            // TODO: -1 username(long) means donot Exist.
        }
        
        if(res.getFinished()){
            System.out.println("UPS Disconnected!");
        }
        return true;
    }

    public synchronized long seqNumGenerator(){
        long temp = globalSeqNum;
        globalSeqNum += 1;
        return temp;
    }

    //After Purchase, we will tell the world to pack and tell the UPS to pick.
    public void purchaseFinished(APurchaseMore apc){
        synchronized (packageMap){
            // Find the Package in the corresponding warehouse
            for(Package p : packageMap.values()){
                if(p.getwarehouseID() != apc.getWhnum()){
                    continue;
                }
                if(!p.getPack().getThingsList().equals(apc.getThingsList())){
                    continue;
                }
                if(!p.status.equals(Package.PROCESSING)){
                    continue;
                }
                System.out.println("Tell the UPS to pick and Tell the World to Pack!");
                p.setStatus(Package.PACKING);
                packPackage(p.getShipID());
                pickPackage(p.getShipID());
                break;
            }
        }
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
    public void pickPackage(long packageId) {
        if(!packageMap.containsKey(packageId)){
            return;
        }
        System.out.println("Tell Truck to pick " + packageId);
        Package pac = packageMap.get(packageId);
        threadPool.execute(()->{
            ACommand.Builder toUps = ACommand.newBuilder();
            //toUps.setIsRequest(true);
            long seqNum = seqNumGenerator();
            long shipId = pac.getShipID();
            APack apc = pac.getPack();
            warehouse w = getWarehouse(pac.getwarehouseID());
            for(AProduct p : apc.getThingsList()){
                newShipment n = getNewShipment(p, w, pac, shipId);
                toUps.addNewShipmentCreated(n);
            }
            // Send msg to UPS
            seqNum = toUps.getNewShipmentCreatedList().get(0).getSeqnum();
            commandToUPS(seqNum, toUps);
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

    public newShipment getNewShipment(AProduct p, warehouse w, Package pac, long shipId){
        newShipment.Builder newShip = newShipment.newBuilder();
        newShip.setShipID(shipId);
        product.Builder pBuilder = product.newBuilder();
        pBuilder.setProductID(p.getId());
        pBuilder.setDescription(p.getDescription());
        pBuilder.setCount(p.getCount());
        newShip.setProductInfo(pBuilder.build());
        newShip.setDestinationInfo(pac.getDestination());
        newShip.setWarehouseInfo(w);
        long seq = seqNumGenerator();
        newShip.setSeqnum(seq);
        return newShip.build();
    }
    

    // We will set the Package and When everything done, load it.
    public void toLoadPackage(long packageId){
        if(!packageMap.containsKey(packageId)){
            return;
        }
        System.out.println("Ready To Load " + packageId);
        Package pac = packageMap.get(packageId);
        pac.setStatus(Package.PACKED);
        // The Package status needs to be PACKED and the Truck need to be ready.
//        if(pac.getTruckID() != -1){
//            LoadPackage(packageId);
//        }
    }
    public void LoadPackage(List<Long> packageIds){
        for(long id :packageIds){
            if(!packageMap.containsKey(id)){
                packageIds.remove(id);
            }
        }
        if(packageIds.size()== 0) {
            return;
        }
        System.out.println("The UPS is Loading");

        threadPool.execute(()->{
            ACommands.Builder ab = ACommands.newBuilder();
            for(long packageid : packageIds) {
                Package p = packageMap.get(packageIds);
                p.setStatus(Package.LOADING);
                long seq = seqNumGenerator();
                APutOnTruck.Builder apto = APutOnTruck.newBuilder();
                apto.setSeqnum(seq);
                apto.setWhnum(p.getwarehouseID());
                apto.setShipid(p.getShipID());
                apto.setTruckid(p.getTruckID());
                ab.addLoad(apto.build());
            }
            long seq = ab.getLoad(0).getSeqnum();
            commandToWorld(seq, ab);
        });
    }
    public void truckLoader(List<Long> packageIds, int truckId){
        for(long id :packageIds){
            if(!packageMap.containsKey(id)){
                packageIds.remove(id);
            }
        }
        if(packageIds.size()== 0) {
            return;
        }
        System.out.println("The Trucking is loading");
        List<Long> toBeLoaded = new ArrayList<>();
        for(long pid : packageIds){
            Package p = packageMap.get(pid);
            p.setTruckID(truckId);
            // wait for all of the package been loaded.
            while(p.getStatus().equals(Package.PACKED)){
                try{
                    Thread.sleep(500);
                }catch(Exception e){
                    System.out.println("In TruckLoader, Error:"+e.toString());
                }
            }
        }
        // The Package status needs to be PACKED and the Truck need to be ready.
        LoadPackage(packageIds);
    }


    public void toDelieverPackage(List<Long> packageIds){
        for(long id :packageIds){
            if(!packageMap.containsKey(id)){
                packageIds.remove(id);
            }
        }
        if(packageIds.size()== 0) {
            return;
        }
        System.out.println("The World have loaded the package and Ready to Deliever");
        Package p = packageMap.get(packageIds);
        p.setStatus(Package.LOADED);
        deliverPackage(packageIds);
    }

    public void deliverPackage(List<Long> packageIds){
        for(long id :packageIds){
            if(!packageMap.containsKey(id)){
                packageIds.remove(id);
            }
        }
        if(packageIds.size()== 0) {
            return;
        }
        System.out.println("The world is Delivering Package");

        threadPool.execute(()->{
            long seq = seqNumGenerator();
            ACommand.Builder ab = ACommand.newBuilder();
            //ab.setIsRequest(true);
            for(long packageId : packageIds){
                Package p = packageMap.get(packageId);
                p.setStatus(Package.DELIVERING);
                APack apc = p.getPack();
                loadedPackage.Builder lb = loadedPackage.newBuilder();
                lb.setSeqnum(seq);
                long trackingNum = QueryFunctions.qTrackingNum(packageId);
                lb.addTrackingNumber(trackingNum);
                lb.setTruckID(p.getTruckID());
                ab.addPackageLoaded(lb.build());
            }
            commandToUPS(seq, ab);
        });
    }
    // Clean the packageMap.
    public void finishDeliver(long packageId){
        if(!packageMap.containsKey(packageId)){
            System.out.println("No Package"+ packageId);
            return;
        }
        System.out.println("Package Been Delivered!");
        packageMap.get(packageId).setStatus(Package.DELIVERED);
        packageMap.remove(packageId);
    }
    public void commandToWorld(long seq, ACommands.Builder command){
        System.out.println("Sending Command to World");
        command.setSimspeed(300);
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
        //command.setIsRequest(false);
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
        System.out.println(command.toString() + "Seqnum is : " + seq);
        
        UPSRequestMap.put(seq, work);
    }

    void buyOrder(long packageID) {
        threadPool.execute(() -> {
            long seq = seqNumGenerator();
            APurchaseMore.Builder newPackage = QueryFunctions.qPackage(packageID);
            if(newPackage==null) return;
            newPackage.setSeqnum(seq);

            APack.Builder builder = APack.newBuilder();
            builder.setWhnum(newPackage.getWhnum());
            builder.addAllThings(newPackage.getThingsList());
            builder.setShipid(packageID);
            builder.setSeqnum(-1);
            Package p = new Package(builder.build());
            System.out.println("The Error MSG");
            p.setStatus(Package.PROCESSING);
            System.out.println("The 2 MSG");
            packageMap.put(packageID, p);

            commandToWorld(seq, ACommands.newBuilder().addBuy(newPackage));
        });
    }


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
            ACommands.Builder ares = ACommands.newBuilder();
            for(long s : seq){
                ares.addAcks(s);
            }
            synchronized(output){
                System.out.println("Now Send Acks to the World.");
                sendMSG(ares, output);    
            }
        }
    }
    public void UPSAckSender(UCommand res) throws IOException{
        ArrayList<Long> seq = new ArrayList<>();

        for(newShipmentResponse ns : res.getTrackingNumberCreatedList()){
            seq.add(ns.getSeqnum());
        }
        for(truckArrival ta : res.getArrivedList()){
            seq.add(ta.getSeqnum());
        }
        for(deliveredPackage dp : res.getDeliveredList()){
            seq.add(dp.getSeqnum());
        }
        for(changeDeliveryAddress cda : res.getChangeAddressList()){
            seq.add(cda.getSeqnum());
        }
        for(checkUsernameResponse cur : res.getCheckUserList()){
            seq.add(cur.getSeqnum());
        }
        for(Err err: res.getErrorList()){
            seq.add(err.getSeqnum());
        }
        if(seq.size() > 0){
            ACommand.Builder ab = ACommand.newBuilder();
            for(long s : seq){
                ab.addAcks(s);
            }
            //ab.setIsRequest(false);
            OutputStream toUps = UPSocket.getOutputStream();
            synchronized (toUps){
                System.out.println("Send ACKS to UPS");
                sendMSG(ab, toUps);
            }
        }
    }
}
