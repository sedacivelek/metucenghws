import java.awt.*;
public class ChaseClosest extends State {
    private Agent agent;
    public ChaseClosest() {
        super();
        setName("ChaseClosest");
    }

    @Override
    public void step(Agent agent) {
        this.agent = agent;
        move();

    }
    //finds closest order to agent
    private Order findClosest(){
        Order closest = null;
        double minDist = Double.MAX_VALUE;
        for (Order order:getCommon().getOrders()){
                double distance = order.getPosition().distanceTo(agent.getPosition().getIntX(),agent.getPosition().getIntY());
                if(distance<minDist){
                    minDist = distance;
                    closest = order;
                }


        }
        return closest;
    }
    private Position RandomPointGenerator(){
        int posX = getRandomNum(20,Common.getWindowWidth()-20);
        int posY = getRandomNum(Common.getUpperLineY(),400);
        return new Position(posX,posY);
    }
    @Override
    public void move(){ //moves to closest order with random speed
        int posX = agent.getPosition().getIntX();
        int posY = agent.getPosition().getIntY();
        int speedX = getRandomNum(1,5);
        int speedY = getRandomNum(1,5);
        Position gotoPos;
        if(findClosest().getPosition().getIntY()<400){
             gotoPos = findClosest().getPosition();
        }
        else{
            gotoPos = RandomPointGenerator();
        }
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
        agent.getPosition().setX(posX);
        agent.getPosition().setY(posY);
    }
    @Override
    public void stateStatus(Position pos, Graphics2D g2d) {
        g2d.drawString(getName(),pos.getIntX(),pos.getIntY()+90);
    }
    // TODO
}