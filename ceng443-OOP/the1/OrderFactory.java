public abstract class OrderFactory {
    //implemented from buyOrderFactory and sellOrderFactory
    public abstract Order createOrder(Country country);
}