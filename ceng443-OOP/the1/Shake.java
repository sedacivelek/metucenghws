import java.awt.*;
import java.util.Random;

public class Shake extends State {
    private Agent agent;
    private Position center;
    private static Random random = new Random();
    public Shake(Position position) {
        super();
        setName("Shake");
        this.center = position;
    }
    @Override
    public void move(){ //mainly changes position of agent in a bounded area, next position selected randomly
        int boundX = random.nextInt(5), boundY = random.nextInt( 5);
        int posX,posY;
        int chooseShake = random.nextInt(50)%4;
        if(chooseShake==0){
            posX = center.getIntX()+ boundX;
            posY = center.getIntY()+ boundY;
        }
        else if (chooseShake==1){
            posX = center.getIntX()- boundX;
            posY = center.getIntY()- boundY;
        }
        else if (chooseShake==2){
            posX = center.getIntX()+ boundX;
            posY = center.getIntY()- boundY;
        }
        else{
            posX= center.getIntX()- boundX;
            posY = center.getIntY()+ boundY;
        }

        agent.getPosition().setX(posX);
        agent.getPosition().setY(posY);

    }

    @Override
    public void step(Agent agent) {
        this.agent = agent;
        move();

    }

    @Override
    public void stateStatus(Position pos, Graphics2D g2d) {
        g2d.drawString(getName(),pos.getIntX(),pos.getIntY()+90);
    }

}