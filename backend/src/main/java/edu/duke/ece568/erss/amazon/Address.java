package edu.duke.ece568.erss.amazon;

public class Address {
    private int x;
    private int y;

    public Address(int x, int y){
        this.x = x;
        this.y = y;
    }

    public int getX(){
        return this.x;
    }
    public int getY(){
        return this.y;
    }
    
    @Override
    public String toString(){
        return String.format("(%d, %d)", x, y);
    }
}
