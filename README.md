# Event Booking System

An **Event Booking System** implemented in C++ that manages event ticket booking, cancellations, and waitlists while considering loyalty points and VIP status. The system adjusts ticket prices dynamically based on seat availability.

## Features

- **Dynamic Ticket Pricing**: Ticket prices increase based on demand.
- **Priority Booking**:
  - VIP customers have higher priority.
  - Non-VIP customers are sorted by loyalty points.
- **Waitlist Management**: Handles waitlist customers and assigns them seats when cancellations occur.
- **Notifications**: Sends booking and waitlist notifications to customers.
- **Cancelation Support**: Frees up seats for waitlisted customers upon cancellations.
- **Booking Summary**:
  - Displays current bookings.
  - Shows waitlist details.

## How It Works

1. **Booking a Seat**:
   - Calculates the current ticket price based on demand.
   - Assigns a seat to the customer if available; otherwise, adds the customer to the waitlist.
   - Notifies the customer of their booking or waitlist status.

2. **Canceling a Booking**:
   - Frees up the seat and assigns it to the highest-priority waitlist customer.
   - Updates the booking summary and notifications.

3. **Display Options**:
   - View all current bookings.
   - View the waitlist.

## Code Overview

### Classes and Structures

1. **`Customer`**:
   - Stores customer details (`name`, `loyaltyPoints`, `seatNumber`, `isVIP`).
   - Implements priority logic for booking.

2. **`BookingNode`**:
   - Represents a node in the doubly linked list of bookings.

3. **`EventBookingSystem`**:
   - Manages bookings, cancellations, waitlists, and ticket pricing.

### Key Methods

- `bookSeat(string name, int loyaltyPoints, bool isVIP = false)`
- `cancelBooking(int seatNumber)`
- `displayBookings()`
- `displayWaitlist()`
- `calculatePrice()`
- `sendNotification(string customerName, int seatNumber, bool bookingConfirmed)`

### Usage Example

```cpp
int main() {
    EventBookingSystem event(5, 100.0);

    event.bookSeat("Alice", 120);
    event.bookSeat("Bob", 90);
    event.bookSeat("Charlie", 150, true);
    event.bookSeat("Diana", 70);
    event.bookSeat("Eve", 100);

    event.bookSeat("Frank", 80);
    event.bookSeat("Grace", 110, true);

    event.displayBookings();
    event.displayWaitlist();
    event.cancelBooking(2);
    event.displayBookings();
    event.displayWaitlist();

    return 0;
}
