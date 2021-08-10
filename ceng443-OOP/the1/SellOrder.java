import java.awt.*;

public class SellOrder extends Order {
    public SellOrder(Country country, double x, double y) {
        super(x, y);
        setOrderColor(Color.pink);
        setType(1);
        setOrderCountry(country);
    }

    @Override
    public void draw(Graphics2D g2d) {
        super.draw(g2d);
    }

    @Override
    public void step() {
        super.step();
        hitting();
    }

    @Override
    protected void hitting() {
        //order arrives upper line, if enough gold, gold is sold
        super.hitting();
        if(getPosition().getIntY()==Common.getUpperLineY()) {
            double hitWorth = getAmount() * Common.getGoldPrice().getCurrentPrice();
            if (getAmount() <= getOrderCountry().getGold()) {
                getOrderCountry().setCash(getOrderCountry().getCash() + hitWorth);
                getOrderCountry().setGold(getOrderCountry().getGold() - getAmount());

            }
        }
    }

    // TODO
}