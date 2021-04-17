package edu.duke.ece568.erss.amazon;

import edu.duke.ece568.erss.amazon.protos.WorldAmazon.APack;

public class Package {
    public static final String PROCESSING = "processing";
    public static final String FINISHED = "finished";
    public static final String PACKING = "packing";
    public static final String PACKED = "packed";
    public static final String LOADING = "loading";
    public static final String LOADED = "loaded";
    public static final String DELIVERING = "delivering";
    public static final String DELIVERED = "delivered";

    protected long shipID;
    protected int warehouseID;
    protected int truckID;
    protected APack pack;
    protected String status;
    
    public Package(APack pack){
        this.shipID = pack.getShipid();
        this.warehouseID = pack.getWhnum();
        this.pack = pack;
        this.status = "";
        this.truckID = -1;
    }

    public long getShipID() {
        return shipID;
    }

    public int getWarehouseID() {
        return warehouseID;
    }


    public int getTruckID() {
        return truckID;
    }


    public APack getPack() {
        return pack;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
        //TODO: Update Database
    }
}
