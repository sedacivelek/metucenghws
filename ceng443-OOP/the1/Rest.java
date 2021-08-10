import java.awt.*;

public class Rest extends State {
    public Rest() {
        super();
        setName("Rest");
    }

    @Override
    public void step(Agent agent) {
    }


    @Override
    public void stateStatus(Position pos, Graphics2D g2d) {
        g2d.drawString(getName(),pos.getIntX(),pos.getIntY()+90);
    }

    @Override
    public void move() {
        //position wont change
    }
}