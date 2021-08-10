import java.awt.*;
import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.Random;

public class BasicAgent extends Agent {
    // TODO
    private Graphics2D g2d;
    private int stepSize;
    private Random random = new Random();
    private BufferedImage bufferedImage;
    private Common common = Common.getCommon();
    public  void draw(Graphics2D g2d){
        this.g2d = g2d;
        drawImage();
        writeText();
        changeState();
        getState().step(this);
        isOrderStolen();
    }
    public  void step(){
        writeText();
        getState().step(this);
        isOrderStolen();
    }

    public BasicAgent(String name,BufferedImage bufferedImage,double posX,double posY,Country agentCountry) {
        super(posX, posY);
        this.setName(name);
        this.common = Common.getCommon();
        this.bufferedImage = bufferedImage;
        stepSize =0;
        this.setAgentCountry(agentCountry);


    }
    //draw agent photos
    private void drawImage() {
            g2d.drawImage(bufferedImage, (int)getPosition().getX(), (int)getPosition().getY(), null);
        }
    //draw agent texts
    private void writeText(){
        g2d.setColor(Color.black);
        g2d.setFont(new Font("Verdana", Font.BOLD, 20));
        g2d.drawString(getName(),getPosition().getIntX(),getPosition().getIntY()-5);
        g2d.setColor(Color.blue);
        getState().stateStatus(getPosition(),g2d);
        g2d.setColor(Color.red);
        g2d.drawString(""+(int)getStolen(),getPosition().getIntX(),getPosition().getIntY()+110);
    }
    //change states of agents randomly
    private void changeState(){
        stepSize++;
        if(stepSize%100==0){
            int changeState = random.nextInt(50)%4;
            if(changeState==0 && getPosition().getIntY()<250){
                setState(new ChaseClosest());
            }
            else if(changeState==1){
                setState(new Rest());
            }
            else if(changeState==2){
                setState(new Shake(getPosition()));
            }
            else{
                setState(new GotoXY());
            }
            stepSize=0;
        }

    }

    //checks if an order is belong to another country. If it is, check if it catches. If it is, checks if there is enough cash or gold of country.
    //If yes,steal, if no, remove order
    private void isOrderStolen(){
        ArrayList<Order> orders = new ArrayList<>(common.getOrders());
        for(Order order:orders){
            if(order.getOrderCountry()!=getAgentCountry()){
                if(order.getPosition().distanceTo((getPosition().getIntX()+40),(getPosition().getIntY()+40))<=60){
                    if(order.getType()==0){
                        double stolenWorth = order.getAmount()*common.getGoldPrice().getCurrentPrice();
                        if(order.getOrderCountry().getCash()>stolenWorth){
                            order.getOrderCountry().setCash(order.getOrderCountry().getCash()-stolenWorth);
                            setStolen(getStolen()+stolenWorth);
                            getAgentCountry().setCash(getAgentCountry().getCash()+stolenWorth);
                        }

                    }
                    else if(order.getType()==1){
                        if(order.getOrderCountry().getGold()>order.getAmount()){
                            order.getOrderCountry().setGold(order.getOrderCountry().getGold()-order.getAmount());
                            setStolen(getStolen()+order.getAmount()*common.getGoldPrice().getCurrentPrice());
                            getAgentCountry().setGold(getAgentCountry().getGold()+order.getAmount());
                        }
                    }
                    common.getOrders().remove(order);
                }
            }

        }
    }

}