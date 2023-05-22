#include <bits/stdc++.h>
using namespace std;

// to store product details
class Product
{
public:
    string name;
    double price;
    int quantity;
    bool giftWrap;

    Product(string &name, double price, int quantity, bool giftwrap) : name(name), price(price), quantity(quantity), giftWrap(giftwrap) {}
};

// to store discount rules
class Discount
{
public:
    string name;
    double discountAmount;

    Discount(string name, double discountAmount) : name(name), discountAmount(discountAmount) {}
};

class ShoppingCart
{
public:
    vector<Discount> discounts;
    vector<Product> products;

    void addProduct(string name, double price, int quantity, bool giftWrap)
    {
        Product product(name, price, quantity, giftWrap);
        products.push_back(product);
    }

    void addDiscount(string name, double discountAmount)
    {
        Discount discount(name, discountAmount);
        discounts.push_back(discount);
    }

    // calculate total without discount, shipping and wrapp
    double calculateSubtotal()
    {
        double subtotal = 0.0;
        for (auto &product : products)
        { // products an array of Product
            subtotal += product.price * product.quantity;
        }
        return subtotal;
    }

    double calculateDiscount(int &disno);

    double calculateExcessQuantityDiscount(int baseQuantity, int maxQuantity, double discountPercentage);

    double calculateGiftWrapFee();

    long calculateShippingFee();

    double calculateTotal();

    void displayCartDetails();
};

double ShoppingCart ::calculateDiscount(int &disno)
{
    double maxDiscount = 0.0;
    double currentDiscount = 0.0; // can we take it out of the loop
    for (auto &discount : discounts)
    { // discounts is an vector
        if (discount.name == "flat_10_discount")
        {
            double subtotal = calculateSubtotal();
            if (subtotal > 200.0)
            {
                currentDiscount = 10.0;
                disno = 1;
            }
        }
        else if (discount.name == "bulk_5_discount")
        {
            for (auto &product : products)
            {
                if (product.quantity > 10)
                {
                    currentDiscount = max(currentDiscount, product.price * product.quantity * 0.05);
                    if (currentDiscount == product.price * product.quantity * 0.05)
                        disno = 2;
                }
            }
        }
        else if (discount.name == "bulk_10_discount")
        {
            int totalQuantity = 0;
            for (auto &product : products)
            {
                totalQuantity += product.quantity;
            }
            if (totalQuantity > 20)
            {
                double currentDis = calculateSubtotal() * 0.10;
                currentDiscount = max(currentDiscount, currentDis);
                if (currentDiscount == currentDis)
                    disno = 3;
            }
        }
        else if (discount.name == "tiered_50_discount")
        {
            int totalQuantity = 0;
            int maxProductQuantity = 0;
            for (auto &product : products)
            {
                totalQuantity += product.quantity;
                maxProductQuantity = max(maxProductQuantity, product.quantity);
            }
            if (totalQuantity > 30 && maxProductQuantity > 15)
            {
                double currentDis = calculateExcessQuantityDiscount(15, maxProductQuantity, 0.50);
                currentDiscount = max(currentDiscount, currentDis);
                // currentDiscount = currentDis;
                if (currentDiscount == currentDis)
                    disno = 4;
            }
        }
        maxDiscount = max(maxDiscount, currentDiscount);
    }
    return maxDiscount;
}

double ShoppingCart ::calculateExcessQuantityDiscount(int baseQuantity, int maxQuantity, double discountPercentage)
{
    int excessQuantity = maxQuantity - baseQuantity;
    double excessDiscount = 0.0;
    for (auto &product : products)
    {
        if (product.quantity > baseQuantity)
        {
            excessDiscount += min(excessQuantity, product.quantity - baseQuantity) * product.price * discountPercentage;
        }
    }
    return excessDiscount;
}

double ShoppingCart ::calculateGiftWrapFee()
{
    double giftWrapFee = 0.0;
    for (auto &product : products)
    {
        if (product.giftWrap)
        {
            giftWrapFee += product.quantity;
        }
    }
    return giftWrapFee;
}

long ShoppingCart ::calculateShippingFee()
{
    long totalQuantity = 0;
    for (auto &product : products)
    {
        totalQuantity += product.quantity;
    }
    long packageCount = (totalQuantity + 9) / 10;
    return packageCount * 5.0;
}

double ShoppingCart ::calculateTotal()
{
    double subtotal = calculateSubtotal();
    int disno = 0;
    double discount = calculateDiscount(disno);
    double giftWrapFee = calculateGiftWrapFee();
    double shippingFee = calculateShippingFee();
    return subtotal - discount + giftWrapFee + shippingFee;
}

void ShoppingCart ::displayCartDetails()
{
    cout << "Cart Details:\n";
    for (auto &product : products)
    {
        cout << "Product: " << product.name << " \n";
        cout << "Quantity: " << product.quantity << " \n";
        cout << "Total: $" << product.price * product.quantity << " \n\n";
    }
    cout << "Subtotal: $" << calculateSubtotal() << "\n";

    int disno = 0;
    double discount = calculateDiscount(disno);
    if (discount > 0.0)
    {
        cout << "Discount Applied: " << discounts[disno - 1].name << "\n";
        cout << "Discount Amount: $" << discount << "\n";
    }

    long shippingFee = calculateShippingFee();
    cout << "Shipping Fee: $" << shippingFee << "\n";

    double total = calculateTotal();
    cout << "Total: $" << total << endl;
}

int main()
{
    ShoppingCart cart;

    // Add products to the cart
    cart.addProduct("Product A", 20.0, 0, false);
    cart.addProduct("Product B", 40.0, 0, false);
    cart.addProduct("Product C", 50.0, 0, false);

    // Add discounts to the cart
    cart.addDiscount("flat_10_discount", 10.0);
    cart.addDiscount("bulk_5_discount", 0.05);
    cart.addDiscount("bulk_10_discount", 0.10);
    cart.addDiscount("tiered_50_discount", 0.50);

    // Prompt for product quantities and gift wrap information
    for (auto &product : cart.products)
    {
        cout << "Enter the quantity of " << product.name << ":";
        cin >> product.quantity;

        cout << "Is " << product.name << " wrapped as a gift? (1 for YES, 0 for NO): ";
        cin >> product.giftWrap;
    }

    cart.displayCartDetails();

    return 0;
}
