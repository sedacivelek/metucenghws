import java.awt.*;

public class GotoXY extends State {
    private Position gotoPos;
    private Agent agent;
    public GotoXY() {
        super();
        setName("GotoXY");
        gotoPos = RandomPointGenerator();

    }
    //select random point to go
    private Position RandomPointGenerator(){
        int posX = getRandomNum(20,Common.getWindowWidth()-20);
        int posY = getRandomNum(Common.getUpperLineY(),400);
        return new Position(posX,posY);
    }

    @Override
    public void step(Agent agent) {
        this.agent = agent;
        move();

    }
    @Override
    public void move(){ // move to selected position with random speed
        int posX = agent.getPosition().getIntX();
        int posY = agent.getPosition().getIntY();
        int speedX = getRandomNum(1,5);
        int speedY = getRandomNum(1,5);
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