/*
 * This Java source file was generated by the Gradle 'init' task.
 */
package edu.duke.ece568.erss.amazon;

public class App {
    public String getGreeting() {
        return "Hello world.";
    }

    public static void main(String[] args) {
        //System.out.println(new App().getGreeting());
        AmazonServer s = new AmazonServer();

        while(true){
            try{
                //s.runAll();
                s.initiaition();
                break;
            }catch(Exception e){
                System.out.println(e.getMessage());
            }
        }
        s.runAll();
    }
}
