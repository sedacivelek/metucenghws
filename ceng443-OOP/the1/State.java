
import java.awt.*;
import java.util.Random;

public abstract class State {
    private String name;
    private Common common;
    private Random random = new Random();
    // TODO

    public State() {
        this.common = Common.getCommon();
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Common getCommon() {
        return common;
    }
    //it was in more than one subclass, decided to write in here
    public int getRandomNum(int min,int max){
        return random.nextInt(max-min)+min;
    }

    public abstract void step(Agent agent);
    //methods below are implemented different in each subclass, so decided to make it abstract
    public abstract void stateStatus(Position pos, Graphics2D g2d);
    public abstract void move();

}