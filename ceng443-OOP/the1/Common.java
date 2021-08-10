
import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Common {
    private static final String title = "Gold Wars";
    private static final int windowWidth = 1300 ;
    private static final int windowHeight = 800;

    private static final GoldPrice goldPrice = new GoldPrice(440, 42);

    private static final Random randomGenerator = new Random(1234);
    private static final int upperLineY = 60;
    private static ArrayList<Country> countries;
    private static ArrayList<String> countryPhotos;
    private static ArrayList<String> countryNames;
    private static ArrayList<String> countryNicks;
    private static Common common;
    private static ArrayList<Agent> agents;
    private static ArrayList<String> agentPhotos;
    private static ArrayList<String> agentNames;
    private static ArrayList<BufferedImage> bufferedImages;
    private static ArrayList<BufferedImage> countryBufferedImages;
    private static ArrayList<Order> orders;
    static  {
        // TODO: Here, you can initialize the fields you have declared
        countries = new ArrayList<>();
        countryPhotos = new ArrayList<>();
        countryNames = new ArrayList<>();
        countryNicks = new ArrayList<>();
        bufferedImages = new ArrayList<>();
        countryBufferedImages = new ArrayList<>();
        orders = new ArrayList<>();

        //country photo paths
        countryPhotos.add("images/usa.jpg"); countryPhotos.add("images/israel.jpg");
        countryPhotos.add("images/turkey.jpg"); countryPhotos.add("images/russia.jpg"); countryPhotos.add("images/china.jpg");

        //names of the countries
        countryNames.add("USA"); countryNames.add("Israel"); countryNames.add("Turkey"); countryNames.add("Russia"); countryNames.add("China");
        countryNicks.add("US"); countryNicks.add("IL");countryNicks.add("TR");countryNicks.add("RU");countryNicks.add("CN");

        double posX=40; //define position values
        double posY=500;
        double photoWidth=100;
        int initialGold = 50;
        int initialCash = 10000;
        double initialWorth = initialCash+initialGold*(getGoldPrice().getCurrentPrice());
        Country country;

        //Read country images just one time, hence speed of the simulation will not be affected
        for (int i=0;i<countryNames.size();i++) {
            try {
                BufferedImage bufferedImage = ImageIO.read(new File(countryPhotos.get(i)));
                BufferedImage newImage = new BufferedImage(80, 80, BufferedImage.TRANSLUCENT);
                Graphics2D g = newImage.createGraphics();
                g.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
                g.drawImage(bufferedImage, 0, 0, 80, 80, null);
                g.dispose();
                countryBufferedImages.add(newImage);

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        for( int i=0;i<countryNames.size();i++) {
            country = new Country(countryNames.get(i),countryNicks.get(i),countryBufferedImages.get(i),posX,posY);
            country.setGold(initialGold);
            country.setCash(initialCash);
            country.setWorth(initialWorth);
            countries.add(country);
            posX += photoWidth+150;
        }

        //Agent initialization
        agents = new ArrayList<>();
        agentPhotos = new ArrayList<>();
        agentNames = new ArrayList<>();

        //agent photo paths
        agentPhotos.add("images/cia.png"); agentPhotos.add("images/mossad.png");agentPhotos.add("images/mit.png");
        agentPhotos.add("images/svr.png"); agentPhotos.add("images/mss.png");

        //agent names
        agentNames.add("CIA"); agentNames.add("Mossad"); agentNames.add("MIT"); agentNames.add("SVR"); agentNames.add("MSS");

        //initial values
        posX = 50; posY = 300;
        photoWidth =30;
        Agent agent;
        //Read agent images just one time, hence speed of the simulation will not be affected
        for (int i=0;i<agentNames.size();i++) {
            try {
                BufferedImage bufferedImage = ImageIO.read(new File(agentPhotos.get(i)));
                BufferedImage newImage = new BufferedImage(80, 80, BufferedImage.TRANSLUCENT);
                Graphics2D g = newImage.createGraphics();
                g.setRenderingHint(RenderingHints.KEY_INTERPOLATION, RenderingHints.VALUE_INTERPOLATION_BILINEAR);
                g.drawImage(bufferedImage, 0, 0, 80, 80, null);
                g.dispose();
                bufferedImages.add(newImage);

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        for (int i=0;i<agentNames.size();i++){
            agent = new Expert(new Master(new Novice(new BasicAgent(agentNames.get(i),bufferedImages.get(i),posX,posY,countries.get(i)))));
            agents.add(agent);
            posX += photoWidth+230;

        }




    }

    // getters
    public static String getTitle() { return title; }
    public static int getWindowWidth() { return windowWidth; }
    public static int getWindowHeight() { return windowHeight; }
    public static GoldPrice getGoldPrice() {
        return goldPrice; }
    public static Random getRandomGenerator() { return randomGenerator; }
    public static int getUpperLineY() { return upperLineY; }
    public static List<Country> getCountries() {
        return countries;
    }
    public static List<Agent> getAgents() {
        return agents;
    }
    public static Common getCommon() {
        if(common==null) common = new Common();
        return common;
    }
    public static void stepAllEntities() {
        if (randomGenerator.nextInt(200)  == 0){ goldPrice.step();
        }
        // Country, order and agent step
        for(Country country:countries){
            country.step();
        }
        for(Order order: orders){
            order.step();
        }
        deleteOrderIfSuccess();
        for(Agent agent:agents){
            agent.step();
        }

    }



    public static ArrayList<Order> getOrders() {
        return orders;
    }
    //returns orders of a spesific country, its used for random order generation
    public int orderNumberOfCountry(Country country){
        int n=0;
        for(Order order:orders){
            if(order.getOrderCountry()==country)
                n++;
        }
        return n;
    }
    //delete orders if they hit the upper line successfully
    private static void deleteOrderIfSuccess(){
        ArrayList<Order> deleteOrder = new ArrayList<>(orders);
        for(Order order : deleteOrder){
            if(order.getPosition().getIntY()==upperLineY){
                orders.remove(order);
            }
        }
    }
}