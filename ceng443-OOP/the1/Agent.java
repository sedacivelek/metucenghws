import java.awt.*;

public abstract class Agent extends Entity {
    // TODO
    private State state; //agent state
    private String name; //agent name
    private double stolen; //stolen money
    private Country agentCountry; //IA's country


    public abstract void step();
    public abstract void draw(Graphics2D g2d);

    public Agent(double x, double y) {
        super(x, y);
        state = new Rest();
        name = "";
        stolen = 0;
    }
    public Agent(Agent agent){
        super(agent.getPosition().getX(),agent.getPosition().getY());
        state = agent.getState();
        name = agent.getName();
        stolen = agent.getStolen();
        agentCountry = agent.getAgentCountry();

    }
    //getters and setters
    public State getState() {
        return state;
    }

    public void setState(State state) {
        this.state = state;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public double getStolen() {
        return stolen;
    }

    public void setStolen(double stolen) {
        this.stolen = stolen;
    }

    public Country getAgentCountry() {
        return agentCountry;
    }

    public void setAgentCountry(Country agentCountry) {
        this.agentCountry = agentCountry;
    }
}