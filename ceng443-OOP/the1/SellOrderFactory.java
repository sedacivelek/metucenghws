public class SellOrderFactory extends OrderFactory {
    @Override
    public Order createOrder(Country country) {
        return new SellOrder(country, country.getPosition().getX(), country.getPosition().getY());
    }
}