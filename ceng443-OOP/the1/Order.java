import java.awt.*;
import java.util.Random;

public abstract class Order extends Entity {
    private Country orderCountry;
    private Color orderColor;
    private int orderSpeed;
    private int type;
    private int amount;
    private Position gotoPos;
    private Common common = Common.getCommon();
    private Random random = new Random();
    private Graphics2D g2d;
    //constructor
    public Order(double x, double y) {
        super(x, y);
        orderCountry = null;
        orderColor = null;
        orderSpeed = random.nextInt(10);
        type=-1;
        amount = getRandomNum(1,6);
        gotoPos = RandomPointGenerator();
    }
    //getters and setters
    public Country getOrderCountry() {
        return orderCountry;
    }

    public void setOrderCountry(Country orderCountry) {
        this.orderCountry = orderCountry;
    }

    public Color getOrderColor() {
        return orderColor;
    }

    public void setOrderColor(Color orderColor) {
        this.orderColor = orderColor;
    }

    public int getOrderSpeed() {
        return orderSpeed;
    }

    public void setOrderSpeed(int orderSpeed) {
        this.orderSpeed = orderSpeed;
    }

    public int getType() {
        return type;
    }

    public void setType(int type) {
        this.type = type;
    }

    public int getAmount() {
        return amount;
    }

    public void setAmount(int amount) {
        this.amount = amount;
    }



    private int getRandomNum(int min, int max){
        return random.nextInt(max-min)+min;
    }
    //return a random point
    private Position RandomPointGenerator(){
        int posX = getRandomNum(1,Common.getWindowWidth());
        int posY = Common.getUpperLineY();
        return new Position(posX,posY);
    }
    @Override
    public void draw(Graphics2D g2d) {
        this.g2d = g2d;
        printOrder();
    }
    //draws buy and sell orders
    private void printOrder(){
        g2d.setColor(orderColor);
        g2d.fillOval(getPosition().getIntX(),getPosition().getIntY(),20,20);
        g2d.setFont(new Font("Verdana",Font.BOLD,10));
        g2d.drawString(orderCountry.getNick(),getPosition().getIntX(),getPosition().getIntY());
        g2d.setColor(Color.black);
        g2d.drawString(""+amount,getPosition().getIntX()+7,getPosition().getIntY()+14);

    }
    //overriden in buyOrder and sellOrder
    protected void hitting(){

    }
    private void orderMove(){
        int posX = getPosition().getIntX();
        int posY = getPosition().getIntY();
        int speedX = getRandomNum(0,2);
        int speedY = getRandomNum(0,2);
        if(Math.abs(gotoPos.getIntX()-posX)>speedX){
            if(gotoPos.getIntX()-posX>0){
                posX += speedX;
            }
            else{
                posX -= speedX;
            }
        }
        else{
            posX = gotoPos.getIntX();
        }
        if(Math.abs(gotoPos.getIntY()-posY)>speedY){
            if(gotoPos.getIntY()-posY>0){
                posY += speedY;
            }
            else{
                posY -= speedY;
            }
        }
        else{
            posY = gotoPos.getIntY();
        }
        getPosition().setX(posX);
        getPosition().setY(posY);
    }

    @Override
    public void step() {
        orderMove();
    }

}