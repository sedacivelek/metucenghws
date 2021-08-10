import java.awt.*;

public class Master extends AgentDecorator {
    private Graphics2D g2d;
    public Master(Agent agent) {
        super(agent);
        setDecoratorAgent(agent);
    }

    @Override
    public void draw(Graphics2D g2d) {
        this.g2d = g2d;
        if(getDecoratorAgent().getStolen()>4000){ //draw yellow rectangle
            int shape = 15;
            g2d.setColor(Color.yellow);
            g2d.fillRect(getDecoratorAgent().getPosition().getIntX()+17,getDecoratorAgent().getPosition().getIntY()-40,shape,shape);
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