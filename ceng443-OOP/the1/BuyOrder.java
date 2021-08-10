import java.awt.*;

public class BuyOrder extends Order {
    public BuyOrder(Country country,double x,double y) {
        super(x, y);
        setOrderColor(Color.green);
        setType(0);
        setOrderCountry(country);
    }

    @Override
    public void draw(Graphics2D g2d) {
        super.draw(g2d);
    }

    @Override
    public void step(){
        super.step();
        hitting();
    }
    @Override
    protected void hitting(){
        //buy order arrives upper line, if enough cash, gold is bought
        super.hitting();
        if(getPosition().getIntY()==Common.getUpperLineY()){
            double hitWorth = getAmount()* Common.getGoldPrice().getCurrentPrice();
            if(hitWorth <=getOrderCountry().getCash()){
                getOrderCountry().setWorth(getOrderCountry().getCash()- hitWorth);
                getOrderCountry().setGold(getOrderCountry().getGold()+ getAmount());

            }
        }

    }

    // TODO
}