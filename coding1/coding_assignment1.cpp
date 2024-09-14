// File name: Coding_assignment1.cpp
// Date: 04-13-2024
// Program purpose: This program is to process movie theater tickets


#include <iostream>
#include <string>
using namespace std;

const int Ksize = 8; // The number of movies that can be stored in the MovieTicketMaster

class Movie {
private:
    string moviename;
    int seatsavailable;
    double ticketprice;

public:
    Movie() : moviename("Mystery Movie"), seatsavailable(100), ticketprice(14.99) {}
    //constructor for setting movie details
    Movie(string name, int seats, double price) : moviename(name), seatsavailable(seats), ticketprice(price) {}
    ~Movie() {
        cout << "Movie " << moviename << " is no longer showing..." << endl;
    }
// Display movie details
    void Display() const {
        cout << "Movie Name: " << moviename << endl;
        cout << "Seats Available: " << seatsavailable << endl;
        cout << "Ticket Price: $" << ticketprice << endl;
    }
// Purchase tickets
    double PurchaseTicket(int num_tickets) {
        if (num_tickets > seatsavailable) return -1;
        seatsavailable -= num_tickets;
        return num_tickets * ticketprice;
    }


    string get_movie_name() const { return moviename; }
    void set_movie_name(string name) { moviename = name; }
    int get_seats_available() const { return seatsavailable; }
    void set_seats_available(int seats) { seatsavailable = seats; }
    double get_ticket_price() const { return ticketprice; }
    void set_ticket_price(double price) { ticketprice = price; }
};

class MovieTicketMaster {
private:
    string theatername;
    string theaterlocation;
    int moviecount;
    Movie movielist[Ksize];

public:
    static const string DefaultTheaterName;
    static const string DefaultTheaterLocation;

    MovieTicketMaster() : theatername(DefaultTheaterName), theaterlocation(DefaultTheaterLocation), moviecount(0) {}
    MovieTicketMaster(string name, string location, int count) : theatername(name), theaterlocation(location), moviecount(count) {}
    ~MovieTicketMaster() {
        cout << "Theater " << theatername << " at " << theaterlocation << " is currently closed." << endl;
    }
    string get_theater_name() const { return theatername; }
    string get_theater_location() const { return theaterlocation; }
    void Init() {
        string names[Ksize] = {"Dune 2", "Kung fu panda 4", "Monkey Man", "Godzilla x Kong: The New Empire", "Spider-Man: Across the Spider-Verse", "The Batman", "Oppenheimer", "Barbie"};
        int seats[Ksize] = {200, 150, 120, 100, 160, 110, 130, 100};
        double prices[Ksize] = {13.99, 12.99, 11.99, 10.99, 13.59, 11.59, 10.59, 11.99};

        for (int i = 0; i < Ksize; i++) {
            movielist[i].set_movie_name(names[i]);
            movielist[i].set_seats_available(seats[i]);
            movielist[i].set_ticket_price(prices[i]);
        }
        moviecount = Ksize;
    }
// Display all movies
    void ViewMovies() const {
        for (int i = 0; i < moviecount; i++) {
            movielist[i].Display();
            cout << endl;
        }
    }
// Search for a movie by name
    Movie* SearchMovie(const string& name) {
        for (int i = 0; i < moviecount; i++) {
            if (movielist[i].get_movie_name() == name) return &movielist[i];
        }
        return nullptr;
    }
// Purchase tickets for a movie
    double PurchaseTicket(const string& name, int num_tickets) {
        Movie* movie = SearchMovie(name);
        if (!movie) {
            cout << "Movie not found." << endl;
            return 0;
        }
        double total = movie->PurchaseTicket(num_tickets);
        if (total == -1) {
            cout << "The show has been sold out or not enough tickets available per your request!" << endl;
            return 0;
        }
        return total;
    }
};

const string MovieTicketMaster::DefaultTheaterName = "AMC";
const string MovieTicketMaster::DefaultTheaterLocation = "My City";

int main() {
    MovieTicketMaster master("My Theater", "City Center", 0);
    master.Init();
    int choice;
    do {
        cout << "\nMOVIE TICKET MASTER" << endl;
        cout << "Theater: " << master.get_theater_name() << " at " << master.get_theater_location() << endl;
        cout << "1. View all movies" << endl;
        cout << "2. Search a movie" << endl;
        cout << "3. Purchase a ticket" << endl;
        cout << "4. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: master.ViewMovies(); break;
            case 2: {
                string moviename;
                cout << "Enter movie name: ";
                cin.ignore();
                getline(cin, moviename);
                Movie* movie = master.SearchMovie(moviename);
                if (movie) movie->Display();
                else cout << "Movie not found." << endl;
                break;
            }
            case 3: {
                string moviename;
                cout << "Enter movie name: ";
                cin.ignore();
                getline(cin, moviename);
                int num_tickets;
                cout << "Enter number of tickets to purchase: ";
                cin >> num_tickets;
                double total = master.PurchaseTicket(moviename, num_tickets);
                if (total > 0) cout << "Total cost: $" << total << endl;
                break;
            }
            case 4: cout << "Exiting program." << endl; break;
            default: cout << "Invalid choice. Please try again." << endl; break;
        }
    } while (choice != 4);

    return 0;
}
