// ============================================================================
// Authors: Ali Peera, Michael Milligan, Taylor Tillander
// 
// Description: Takes in CSV file containing users' Spotify data using tabs as 
// delimiters and stores the data into either an unordered_map or ordered map 
// with key: song/artist name and value: number of total streams. 
// 
// Primary Functions: 
// Display all songs or artists in the maps with their corresponding quantity 
// of streams.
// Display top 'n' songs or artists, ranked by quantity of streams
// Search for a song or artist and output corresponding quantity of streams.
// ============================================================================

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
using namespace std::chrono;

//Convert map to vector for sorting by top artists and songs 
vector<pair<string, int>>
unordered_mapToVector(unordered_map<string, int> inputMap) {
  vector<pair<string, int>> sortedMap(inputMap.begin(), inputMap.end());

  // Sort the vector based on the second element of the pair
  sort(sortedMap.begin(), sortedMap.end(), // nlogn
       [](const pair<string, int> &a, const pair<string, int> &b) {
         return a.second > b.second;
       }); // (comp) function in sort() compares the # of streams from the pair

  return sortedMap;
}

vector<pair<string, int>> ordered_mapToVector(map<string, int> inputMap) {
  vector<pair<string, int>> sortedMap(inputMap.begin(), inputMap.end());

  // Sort the vector based on the second element of the pair
  sort(sortedMap.begin(), sortedMap.end(),
       [](const pair<string, int> &a, const pair<string, int> &b) {
         return a.second > b.second;
       });
      // (comp) function in sort() compares the # of streams from the pair

  return sortedMap;
}


//Print out top n elements 
void printVector(vector<pair<string, int>> inputVector, int counter) {
  int count = 0;
  // Iterate through the vector of pairs
  for (const auto &pair : inputVector) {
    if (count < counter) {
      cout << pair.first << ": " << pair.second << std::endl;
      count++;
    } else {
      break; // Stop after printing the first "counter" elements
    }
  }
}

//Print all elements of unordered_map
void printUnorderedMap(unordered_map<string, int> inputMap) {
  auto start = high_resolution_clock::now(); // Timing
  
  for (const auto &pair : inputMap) {
    cout << pair.first << ": " << pair.second << endl;
  }
  
  auto stop = high_resolution_clock::now(); // Timing
  auto duration = duration_cast<milliseconds>(stop - start);
  cout << "Time to print Unordered Map: "
       << duration.count() << "ms" << endl;
}

//Print all elements of ordered map
void printOrderedMap(map<string, int> inputMap) {
  auto start = high_resolution_clock::now(); // Timing
  
  for (const auto &pair : inputMap) {
    cout << pair.first << ": " << pair.second << endl;
  }

  auto stop = high_resolution_clock::now(); // Timing
  auto duration = duration_cast<milliseconds>(stop - start);
  cout << "Time to print Ordered Map " 
       << duration.count() << "ms" << endl;
}

//Create ordered map
map<string, int> createOrderedMap(string fileName, string keySignifier) {
  ifstream file(fileName);
  // If the file name could not open
  if (!file.is_open()) {
    cerr << "Error: Could not open file " << fileName << endl;
    return {};
  }
  map<string, int> countMap;
  // "line" is a row in the csv, separating each stream
  string line;

  // Read the header line to find the index of the "title" column
  getline(file, line);
  istringstream headerStream(line);
  vector<string> headers;
  string header;

  while (getline(headerStream, header, '\t')) {
    headers.push_back(header);
  }
  // keyIndex is the index of column in the csv that has the desired key
  int keyIndex =
      find(headers.begin(), headers.end(), keySignifier) - headers.begin();

  auto start = high_resolution_clock::now();
  
  // Read the rest of the lines
  while (getline(file, line)) {
    istringstream sstream(line);
    string key;

    // Iterate through until titleIndex is reached, get the title
    for (int i = 0; i <= keyIndex; ++i) {
      getline(sstream, key, '\t');
    }
    // Increment the count in the map
    countMap[key]++;
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  cout << "Time to create Ordered Map: " << duration.count()
       << "ms" << endl;
  
  return countMap;
}


//Create unordered map
unordered_map<string, int> createUnorderedMap(string fileName,
                                              string keySignifier) {

  ifstream file(fileName);
  if (!file.is_open()) { // incase file does not open
    cerr << "Error: Could not open file " << fileName << endl;
    return {};
  }
  unordered_map<string, int> countMap;
  // Row in csv, separating each stream
  string line;

  // Read the header line to find the index of the "title" column
  getline(file, line);
  istringstream headerStream(line);
  vector<string> headers;
  string header;

  while (getline(headerStream, header, '\t')) {
    headers.push_back(header);
  }
  // keyIndex is the index of column in the csv that has says the desired key
  int keyIndex =
      find(headers.begin(), headers.end(), keySignifier) - headers.begin();

  auto start = high_resolution_clock::now();
  
  // Read the rest of the lines
  while (getline(file, line)) {
    istringstream sstream(line);
    string key;

    // Iterate through until titleIndex is reached, get the title
    for (int i = 0; i <= keyIndex; ++i) {
      getline(sstream, key, '\t');
    }
    // Increment the count in the map
    countMap[key]++;
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(stop - start);
  cout << "Time to create Unordered Map: " << duration.count()
       << "ms" << endl;
  
  return countMap;
}

//Create menu selection
void displayMenu() {
  cout << endl
       << "What would you like to do?" << endl
       << "1. Create Unordered Map" << endl
       << "2. Create Ordered Map" << endl
       << "3. Output Unordered Map" << endl
       << "4. Output Ordered Map" << endl
       << "5. Search Unordered Map" << endl
       << "6. Search Ordered Map" << endl
       << "7. End Program" << endl;
}

int main() {
  // Initializing maps to be used later
  unordered_map<string, int> songUnorderedMap;
  unordered_map<string, int> artistUnorderedMap;
  map<string, int> songMap;
  map<string, int> artistMap;

  // First: convert desired file to istringstream
    cout << "Input File Name: "; // SpotifyData/CombinedSpotifyData.csv
    string fileName;
    getline(cin, fileName);
    string directory;
    directory = "SpotifyData/" + fileName;
    ifstream file(directory);

    while (!file.is_open()) {
        if (!file.is_open()) { // incase file does not open
            cout << "Error: Could not open file " << fileName << endl << endl;
        }
        cout << "Input File Name: ";
        getline(cin, fileName);
        directory = "SpotifyData/" + fileName;
        file.open(directory);
    }

  displayMenu();

  string operation;
  cin >> operation;

  int resultNum;
  int searchNum;

  while (stoi(operation) != 7) {
    // Interpret User Input
    switch (stoi(operation)) {
    // Create Unordered Map
    case 1:
      cout << endl
           << "1. Sort Unordered Map by Song Title" << endl
           << "2. Sort Unordered Map by Artist Name" << endl;
      cin >> operation;

      switch (stoi(operation)) {
      // Sort by Song Title
      case 1:
        // Create unordered map with csv input
        if (songUnorderedMap.empty()) {
      
          songUnorderedMap =
              createUnorderedMap(directory, "master_metadata_track_name");

        } else {
          cout << "This map has already been created!" << endl << endl;
        }
        break;

      // Sort by Artist Name
      case 2:
        if (artistUnorderedMap.empty()) {
          artistUnorderedMap =
              createUnorderedMap(directory, "master_metadata_album_artist_name");
        } else {
          cout << "This map has already been created!" << endl << endl;
        }
        break;

      default:
        cout << "Invalid Input!" << endl;
      }
      break;

    // Create Ordered Map
    case 2:
      cout << endl
           << "1. Sort Ordered Map by Song Title" << endl
           << "2. Sort Ordered Map by Artist Name" << endl;
      cin >> operation;

      switch (stoi(operation)) {
      // Sort by Song Title
      case 1:
        // Create Ordered map with csv input
        if (songMap.empty()) {
          songMap = createOrderedMap(directory, "master_metadata_track_name");
        } else {
          cout << "This map has already been created!" << endl << endl;
        }
        break;

        // Sort by Artist Name
      case 2:
        if (artistMap.empty()) {
          artistMap =
              createOrderedMap(directory, "master_metadata_album_artist_name");
        } else {
          cout << "This map has already been created!" << endl << endl;
        }
        break;

      default:
        cout << "Invalid Input!" << endl;
      }
      break;

    // Output Unordered Map
    case 3:
      cout << endl
           << "1. Display Top Song Titles" << endl
           << "2. Display Top Artist Names" << endl
           << "3. Display All Song Titles" << endl
           << "4. Display All Artist Names" << endl;
      cin >> operation;


      //Ask user how many entries they want outputted
      if (stoi(operation) == 1 || stoi(operation) == 2) {
        cout << endl;
        cout << "Enter the number of results to display: ";
        cin >> resultNum;
        cout << endl;
      }

      switch (stoi(operation)) {
      // Display Song Titles based on top n results
      case 1:
        if (!songUnorderedMap.empty()) {
          auto start = chrono::high_resolution_clock::now();
          
          printVector(unordered_mapToVector(songUnorderedMap), resultNum); 
          
          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<microseconds>(stop - start);
          cout << "Time to Sort Unordered Map and print " << resultNum 
               << " songs: " << duration.count() << " microseconds" << endl;
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        
        break;

      // Display Artist Names based on top n artists
      case 2:
        if (!artistUnorderedMap.empty()) {
          auto start = chrono::high_resolution_clock::now();
          
          printVector(unordered_mapToVector(artistUnorderedMap), resultNum);

          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<microseconds>(stop - start);
          cout << endl << "Time to Sort Unordered Map and print " << resultNum 
               << " artists: " << duration.count() << " microseconds" << endl;
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        break;

        //Display all songs 
      case 3: 
        if (!songUnorderedMap.empty()) {
          printUnorderedMap(songUnorderedMap);
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        break;

        //Display all artists
      case 4:
        if (!artistUnorderedMap.empty()) {
          printUnorderedMap(artistUnorderedMap);
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        break;

      default:
        cout << "Invalid Input!" << endl;
      }
      break;

    // Output Ordered Map
    case 4:
      cout << endl
           << "1. Display Top Song Titles" << endl
           << "2. Display Top Artist Names" << endl
           << "3. Display All Song Titles" << endl
           << "4. Display All Artist Names" << endl;
      cin >> operation;

      //Ask user how many entries they want outputted
      if (stoi(operation) == 1 || stoi(operation) == 2) {
        cout << endl;
        cout << "Enter the number of results to display: ";
        cin >> resultNum;
        cout << endl;
      }

      switch (stoi(operation)) {
      // Display Song Titles based on top n results
      case 1:
        if (!songMap.empty()) {
          auto start = high_resolution_clock::now();
          
          printVector(ordered_mapToVector(songMap), resultNum);

          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<microseconds>(stop - start);
          cout << "Time to Sort Ordered Map and print " << resultNum 
               << " songs: " << duration.count() << " microseconds" << endl;
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        break;

      // Display Artist Names based on top n results 
      case 2:
        if (!artistMap.empty()) {
          auto start = high_resolution_clock::now();
          
          printVector(ordered_mapToVector(artistMap), resultNum);

          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<microseconds>(stop - start);
          cout << "Time to Sort Ordered Map and print " << resultNum 
               << " artists: " << duration.count() << " microseconds" << endl;
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        break;

        //print all songs 
      case 3:
        if (!songMap.empty()) {
          printOrderedMap(songMap);
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        break;

        //print all artists 
      case 4:
        if (!artistMap.empty()) {
          printOrderedMap(artistMap);
        } else {
          cout << "This map has not been created yet!" << endl << endl;
        }
        break;

      default:
        cout << "Invalid Input!" << endl;
      }
      break;

    // search unordered
    case 5: {
      cout << endl;
      cout << "1. Search by Song" << endl << "2. Search by Artist" << endl;
      cin >> searchNum;
      cout << endl;

      string searchInput;
      int streamCount;
      switch (searchNum) {

        // Search by Song
        case 1: {
          if (songUnorderedMap.empty()) {
            cout << "Song-sorted map has not yet been created!" << endl;
            break;
          }
          cout << "Song Title: ";
          cin.ignore();
          getline(cin, searchInput);
  
          //Create timer to track how long it takes to search 
          auto start = high_resolution_clock::now();
          // Search; determines time complexity
          streamCount = songUnorderedMap[searchInput]; 
  
          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<nanoseconds>(stop - start);
  
          cout << endl;
  
          if (streamCount > 0) {
            cout << "Number of streams: " << streamCount << endl;
          } else {
            cout << "Song Title Not Found!" << endl;
          }
  
           cout << "Time to Search Unordered Map: " 
                << duration.count() << "ns" << endl;
          break;
        }
          // Search by Artist
        case 2: {
          if (artistUnorderedMap.empty()) {
            cout << "Artist-sorted map has not yet been created!" << endl;
            break;
          }
          cout << "Artist Name: ";
          cin.ignore();
          getline(cin, searchInput);
  
          //Create timer to track how long it takes to search 
          auto start = high_resolution_clock::now();
  
          streamCount =
              artistUnorderedMap[searchInput]; // Determines this time complexity
  
          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<nanoseconds>(stop - start);
  
          cout << endl;
  
          if (streamCount > 0) {
            cout << "Number of streams: " << streamCount << endl;
          } else {
            cout << "Artist Name Not Found!" << endl;
          }
          cout << "Time to Search Unordered Map: " 
               << duration.count() << "ns" << endl;
          
          break;
        }
      }
      break;
    }

    // search ordered map
    case 6: {
      cout << endl;
      cout << "1. Search by Song" << endl << "2. Search by Artist" << endl;
      cin >> searchNum;
      cout << endl;

      string searchInput;
      int streamCount;
      switch (searchNum) {

        // Search by Song
        case 1: {
          if (songMap.empty()) {
            cout << "Song-sorted map has not yet been created!" << endl;
            break;
          }
          cout << "Song Title: ";
          cin.ignore();
          getline(cin, searchInput);
  
          //Create timer to track how long it takes to search 
          auto start = high_resolution_clock::now();
  
          streamCount = songMap[searchInput]; // Determines this time complexity
  
          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<nanoseconds>(stop - start);
         
  
          if (streamCount > 0) {
            cout << "Number of streams: " << streamCount << endl;
          } else {
            cout << "Song Title Not Found!" << endl;
          }
  
          cout << "Time to Search Ordered Map: " << duration.count() << "ns"
               << endl;
          cout << endl;
          
          break;
        }
          // Search by Artist
        case 2: {
          if (artistMap.empty()) {
            cout << "Artist-sorted map has not yet been created!" << endl;
            break;
          }
          cout << "Artist Name: ";
          cin.ignore();
          getline(cin, searchInput);
  
          //Create timer to track how long it takes to search 
          auto start = high_resolution_clock::now();
  
          streamCount = artistMap[searchInput]; // Determines this time complexity
  
          auto stop = high_resolution_clock::now();
          auto duration = duration_cast<nanoseconds>(stop - start);
      
          if (streamCount > 0) {
            cout << "Number of streams: " << streamCount << endl;
          } else {
            cout << "Artist Name Not Found!" << endl;
          }
          cout << "Time to Search Ordered Map: " << duration.count() << "ns"
               << endl;
          cout << endl;
          
          break;
        }
      }
      break;
    }

    // End Program
    case 7:
      cout << "Thank you for using MusicStats!" << endl;
      break;

    default:
      cout << "Invalid Input!" << endl;
    }

    displayMenu();
    cin >> operation;
  }

  return 0;
}
