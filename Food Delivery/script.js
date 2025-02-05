
let cart = [];

function addToCart(itemName) {
    // Define the price for each item
    const itemPrices = {
        'Burger': 359,
        'Pizza': 279,
        'Tibes': 343,
        'Ferfer': 179,
        'shero': 210,
        'Tere Sega': 640,
        'Beyayent': 249,
        'Kitfo': 480,
        'Doro Wet': 580
    };
    const itemPrice = itemPrices[itemName];

    if (itemPrice !== undefined) {
        // Add the item to the cart
        cart.push({ name: itemName, price: itemPrice });
        updateCartCount();
        updateCartItems();
    } else {
        alert('Item not found!');
    }
}

function orderNow() {
    if (cart.length === 0) {
        alert('Your cart is empty!');
    } else {
        
        window.location.href = "payment.html";
        cart = []; // Clear the cart
        updateCartCount();
        updateCartItems();
    }
}

function updateCartCount() {
    document.getElementById('cart-count').textContent = cart.length;
}

function updateCartItems() {
    const cartItems = document.getElementById('cart-items');
    const totalPriceElement = document.getElementById('total-price');
    let totalPrice = 0;

    // Clear the cart items
    cartItems.innerHTML = '';

    // Loop through each item in the cart
    cart.forEach(item => {
        const div = document.createElement('div');
        div.textContent = `${item.name} - ${item.price} ETB`;
        cartItems.appendChild(div);

        // Add the item price to the total price
        totalPrice += item.price;
    });

    // Update the total price
    if (totalPriceElement) {
        totalPriceElement.textContent = `Total Price: ${totalPrice.toFixed(2)} ETB`;
    }
}

function checkout() {
    if (cart.length === 0) {
        alert('Your cart is empty!');
    } else {
        alert('Order placed successfully!');
        cart = [];
        updateCartCount();
        updateCartItems();
    }
}
