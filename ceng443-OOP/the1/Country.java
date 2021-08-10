import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.Random;

public class Country extends Entity {
    // TODO
    private String name;
    private String nick;
    private double gold;
    private double cash;
    private Common common = Common.getCommon();
    private double worth;
    private static final Random randomGenerator = new Random();
    private Graphics2D g2d;
    private BufferedImage bufferedImage;

    //constructor
    public Country(String name,String nick,BufferedImage bufferedImage,double x, double y) {
        super(x, y);
        this.name = name;
        this.nick = nick;
        gold = 50;
        cash = 10000;
        worth =0;
        this.bufferedImage = bufferedImage;
    }
    
    //getters
    public String getName() {
        return name;
    }

    public double getGold() {
        return gold;
    }

    public double getCash() {
        return cash;
    }

    public double getWorth() {
        return worth;
    }


    public String getNick() {
        return nick;
    }
    //setters

    public void setName(String name) {
        this.name = name;
    }

    public void setGold(double gold) {
        this.gold = gold;
    }

    public void setCash(double cash) {
        this.cash = cash;
    }

    public void setWorth(double worth) {
        this.worth = worth;
    }



    public void setNick(String nick) {
        this.nick = nick;
    }
    //draw country photos
    private void drawImage() {
            g2d.drawImage(bufferedImage, (int)getPosition().getX(), (int)getPosition().getY(), null);
    }
    //draw countries values
    private void writeText(){
        g2d.setColor(Color.BLACK);
        g2d.setFont(new Font("Verdana", Font.BOLD, 25));
        g2d.drawString(name,getPosition().getIntX(),getPosition().getIntY()+130);
        g2d.setColor(Color.YELLOW);
        g2d.drawString(new StringBuilder().append((int)gold).append(" gold").toString(),getPosition().getIntX(),getPosition().getIntY()+155);
        g2d.setColor(Color.GREEN);
        g2d.drawString(new StringBuilder().append((int)cash).append(" cash").toString(),getPosition().getIntX(),getPosition().getIntY()+180);
        worth = cash+gold*common.getGoldPrice().getCurrentPrice();
        g2d.setColor(Color.BLUE);
        g2d.drawString(new StringBuilder().append("Worth: ").append((int)worth).toString(),getPosition().getIntX(),getPosition().getIntY()+205);
    }
    //create randomly selected type of order using order factory
    public Order createOrder() {
        int randomOrder = randomGenerator.nextInt(51);
        OrderFactory orderFactory;
        if (randomOrder % 2 == 0) {
            orderFactory = new BuyOrderFactory();
        } else {
            orderFactory = new SellOrderFactory();
        }
        return orderFactory.createOrder(this);
    }
    //generates order for countries
    public void orderGenerator(){
        for(int i=common.orderNumberOfCountry(this);i<4;i++){
            Order order = createOrder();
            common.getOrders().add(order);

        }
    }
    @Override
    public void draw(Graphics2D g2d){
        this.g2d=g2d;
        drawImage();
        writeText();
    }
    @Override
    public void step() {
        writeText();
        orderGenerator();

    }
}

