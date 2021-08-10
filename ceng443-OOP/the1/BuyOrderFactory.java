public class BuyOrderFactory extends OrderFactory {
    @Override
    public Order createOrder(Country country) {
        return new BuyOrder(country,country.getPosition().getX(),country.getPosition().getY());
    }
    // TODO
}