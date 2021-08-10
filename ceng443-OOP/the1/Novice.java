import java.awt.*;

public class Novice extends AgentDecorator {
    private Graphics2D g2d;
    public Novice(Agent agent) {
        super(agent);
        setDecoratorAgent(agent);
    }

    @Override
    public void draw(Graphics2D g2d) {
        this.g2d = g2d;
        if(getDecoratorAgent().getStolen()>2000){ //draw white rect
            int shape = 15;
            g2d.setColor(Color.white);
            g2d.fillRect(getDecoratorAgent().getPosition().getIntX(),getDecoratorAgent().getPosition().getIntY()-40,shape,shape);


        }
        getDecoratorAgent().draw(g2d);
    }

    @Override
    public void step() {
        getDecoratorAgent().step();
        super.getPosition().setX(getDecoratorAgent().getPosition().getX());
        super.getPosition().setY(getDecoratorAgent().getPosition().getY());
        super.setStolen(getDecoratorAgent().getStolen());
    }
    // TODO
}