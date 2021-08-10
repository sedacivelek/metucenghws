import java.awt.*;

public abstract class AgentDecorator extends Agent {
    private Agent decoratorAgent;

    public AgentDecorator(Agent agent) {
        super(agent);
        decoratorAgent = agent;

    }
    //Implemented different in novice,master and expert
    public abstract void step();
    public abstract  void draw(Graphics2D g2d);

    public Agent getDecoratorAgent() {
        return decoratorAgent;
    }

    public void setDecoratorAgent(Agent decoratorAgent) {
        this.decoratorAgent = decoratorAgent;
    }
    // TODO
}