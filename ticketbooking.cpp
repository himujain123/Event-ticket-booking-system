#include <iostream>
#include <queue>
#include <unordered_map>
#include <list>
#include <string>
#include <thread>
#include <chrono>
using namespace std;

struct Customer {
    string name;
    int loyaltyPoints;
    int seatNumber;
    bool isVIP;

    Customer() : name(""), loyaltyPoints(0), seatNumber(-1), isVIP(false) {}

    Customer(string n, int lp, int seat, bool vip = false) : name(n), loyaltyPoints(lp), seatNumber(seat), isVIP(vip) {}

    bool operator<(const Customer& other) const {
        if (isVIP == other.isVIP) {
            return loyaltyPoints < other.loyaltyPoints;
        }
        return !isVIP;
    }
};

struct BookingNode {
    Customer customer;
    BookingNode* prev;
    BookingNode* next;

    BookingNode(Customer cust) : customer(cust), prev(nullptr), next(nullptr) {}
};

class EventBookingSystem {
private:
    priority_queue<Customer> priorityQueue;
    unordered_map<int, Customer> seatMap;
    list<BookingNode*> bookingList;
    queue<Customer> waitlistQueue;
    int totalSeats;
    int availableSeats;
    double baseTicketPrice;

public:
    EventBookingSystem(int seats, double price) : totalSeats(seats), availableSeats(seats), baseTicketPrice(price) {}

    double calculatePrice() {
        double demandFactor = 1.0 + (static_cast<double>(totalSeats - availableSeats) / totalSeats);
        return baseTicketPrice * demandFactor;
    }

    void bookSeat(string name, int loyaltyPoints, bool isVIP = false) {
        double currentPrice = calculatePrice();
        if (availableSeats > 0) {
            int seatNumber = totalSeats - availableSeats + 1;
            Customer newCustomer(name, loyaltyPoints, seatNumber, isVIP);
            priorityQueue.push(newCustomer);
            seatMap[seatNumber] = newCustomer;
            BookingNode* newBooking = new BookingNode(newCustomer);
            bookingList.push_back(newBooking);
            availableSeats--;
            cout << "Seat booked successfully for " << name << " (Seat No: " << seatNumber << ") at price: ₹" << currentPrice << endl;
            sendNotification(name, seatNumber, true);
        } else {
            cout << "No seats available. Adding " << name << " to the waitlist.\n";
            Customer waitlistedCustomer(name, loyaltyPoints, -1, isVIP);
            waitlistQueue.push(waitlistedCustomer);
            sendNotification(name, -1, false);
        }
    }

    void cancelBooking(int seatNumber) {
        if (seatMap.find(seatNumber) != seatMap.end()) {
            Customer canceledCustomer = seatMap[seatNumber];
            seatMap.erase(seatNumber);
            cout << "Booking canceled for " << canceledCustomer.name << " (Seat No: " << seatNumber << ")\n";
            availableSeats++;

            if (!waitlistQueue.empty()) {
                Customer nextCustomer = waitlistQueue.front();
                waitlistQueue.pop();
                nextCustomer.seatNumber = seatNumber;
                seatMap[seatNumber] = nextCustomer;
                BookingNode* newBooking = new BookingNode(nextCustomer);
                bookingList.push_back(newBooking);
                availableSeats--;
                cout << nextCustomer.name << " from the waitlist has been assigned Seat No: " << seatNumber << endl;
                sendNotification(nextCustomer.name, seatNumber, true);
            }
        } else {
            cout << "Seat number " << seatNumber << " is not booked yet.\n";
        }
    }

    void displayBookings() {
        cout << "\nCurrent Bookings: \n";
        for (auto it = bookingList.begin(); it != bookingList.end(); ++it) {
            BookingNode* node = *it;
            cout << "Name: " << node->customer.name << ", Seat No: " << node->customer.seatNumber
                 << ", Loyalty Points: " << node->customer.loyaltyPoints << ", VIP: " << (node->customer.isVIP ? "Yes" : "No") << endl;
        }
    }

    void displayWaitlist() {
        cout << "\nWaitlist Customers: \n";
        queue<Customer> tempQueue = waitlistQueue;
        while (!tempQueue.empty()) {
            Customer waitlistedCustomer = tempQueue.front();
            tempQueue.pop();
            cout << "Name: " << waitlistedCustomer.name << ", Loyalty Points: " << waitlistedCustomer.loyaltyPoints
                 << ", VIP: " << (waitlistedCustomer.isVIP ? "Yes" : "No") << endl;
        }
    }

    void sendNotification(string customerName, int seatNumber, bool bookingConfirmed) {
        this_thread::sleep_for(chrono::milliseconds(500));
        if (bookingConfirmed) {
            cout << "Notification: Booking confirmed for " << customerName << ". Seat No: " << seatNumber << "!\n";
        } else {
            cout << "Notification: " << customerName << ", you are on the waitlist for the event.\n";
        }
    }
};

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
