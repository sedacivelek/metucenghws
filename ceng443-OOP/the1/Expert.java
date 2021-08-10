import java.awt.*;

public class Expert extends AgentDecorator {

    private  Graphics2D g2d;
    public Expert(Agent agent) {
        super(agent);
        setDecoratorAgent(agent);
    }

    @Override
    public void draw(Graphics2D g2d) {
        this.g2d = g2d;
         if(getDecoratorAgent().getStolen()>6000){ //draw red rectangle
            int shape = 15;
            g2d.setColor(Color.red);
            g2d.fillRect(getDecoratorAgent().getPosition().getIntX()+34,getDecoratorAgent().getPosition().getIntY()-40,shape,shape);
        }
        getDecoratorAgent().draw(g2d);
    }

    @Override
    public void step() {
        getDecoratorAgent().step();

    }
    // TODO
}