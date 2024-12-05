#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>
#include "song.h"
#include "rb_map.h"
#include "min_heap.h"
using namespace std;

int main() {
    ifstream inFile("dataset.csv");
    if (!inFile.is_open()) {
        cout << "Not Opened!" << endl;
        return 1;
    }

    string row_s, track_id, artist_s, artist, album_name, track_name, popularity_s,
            duration_ms_s, explicit_s, danceability_s, energy_s, key_s, loudness_s, mode_s,
            speechiness_s, acousticness_s, instrumentalness_s, liveness_s, valence_s, tempo_s,
            time_signature_s, track_genre, junk;
    rb_map<double> songs_map;
    min_heap songs_heap;

    unordered_map<string, Song> types;
    // danceability, energy, loudness, speechiness, acousticness, instrumentalness, liveness, valence, tempo
    types["ENFP"] = Song(0.85, 0.8, 0.75, 0.7, 0.4, 0.3, 0.9, 0.85, 0.85);
    types["ENTP"] = Song(0.8, 0.85, 0.8, 0.75, 0.3, 0.4, 0.85, 0.8, 0.85);
    types["ENTJ"] = Song(0.7, 0.9, 0.85, 0.6, 0.4, 0.5, 0.75, 0.8, 0.9);
    types["ENFJ"] = Song(0.85, 0.85, 0.75, 0.8, 0.5, 0.3, 0.85, 0.9, 0.85);

    types["ESFP"] = Song(0.95, 0.9, 0.8, 0.6, 0.3, 0.2, 0.95, 0.9, 0.9);
    types["ESTP"] = Song(0.95, 0.95, 0.9, 0.6, 0.3, 0.2, 0.95, 0.9, 0.95);
    types["ESTJ"] = Song(0.8, 0.85, 0.8, 0.5, 0.4, 0.4, 0.8, 0.75, 0.85);
    types["ESFJ"] = Song(0.85, 0.8, 0.75, 0.7, 0.5, 0.3, 0.85, 0.9, 0.8);

    types["INFP"] = Song(0.5, 0.4, 0.3, 0.5, 0.8, 0.6, 0.4, 0.3, 0.5);
    types["INTP"] = Song(0.45, 0.45, 0.4, 0.4, 0.6, 0.75, 0.4, 0.35, 0.45);
    types["INTJ"] = Song(0.4, 0.5, 0.3, 0.3, 0.7, 0.8, 0.3, 0.2, 0.4);
    types["INFJ"] = Song(0.4, 0.4, 0.3, 0.5, 0.7, 0.7, 0.4, 0.4, 0.5);

    types["ISFP"] = Song(0.7, 0.6, 0.5, 0.4, 0.7, 0.6, 0.6, 0.6, 0.6);
    types["ISTP"] = Song(0.6, 0.6, 0.5, 0.3, 0.5, 0.8, 0.5, 0.5, 0.55);
    types["ISTJ"] = Song(0.5, 0.55, 0.45, 0.4, 0.6, 0.7, 0.4, 0.4, 0.5);
    types["ISFJ"] = Song(0.6, 0.5, 0.4, 0.5, 0.8, 0.6, 0.5, 0.5, 0.5);

    string type, genre;
    // Is All Caps
    cin >> type;
    cin >> genre;

    getline(inFile, row_s, ',');
    getline(inFile, track_id, ',');
    getline(inFile, artist, ',');
    getline(inFile, album_name, ',');
    getline(inFile, track_name, ',');
    getline(inFile, popularity_s, ',');
    getline(inFile, duration_ms_s, ',');
    getline(inFile, explicit_s, ',');
    getline(inFile, danceability_s, ',');
    getline(inFile, energy_s, ',');
    getline(inFile, key_s, ',');
    getline(inFile, loudness_s, ',');
    getline(inFile, mode_s, ',');
    getline(inFile, speechiness_s, ',');
    getline(inFile, acousticness_s, ',');
    getline(inFile, instrumentalness_s, ',');
    getline(inFile, liveness_s, ',');
    getline(inFile, valence_s, ',');
    getline(inFile, tempo_s, ',');
    getline(inFile, time_signature_s, ',');
    getline(inFile, track_genre);
    // FIXME: Rid of Try-Catch When Done
    while (!inFile.eof()) {
        getline(inFile, row_s, ',');
        if (row_s.empty()) {
        getline(inFile, junk);
            break;
        }
        int row = stoi(row_s);

        getline(inFile, track_id, ',');

        // Separated by ';'
        getline(inFile, artist_s, ',');
        if (artist_s.empty()) {
            getline(inFile, junk);
            continue;
        }
        vector<string> artists;
        // https://cplusplus.com/reference/string/string/find/
        size_t pos = artist_s.find(';');
        while (pos != string::npos) {
            artist = artist_s.substr(0, pos);
            artists.push_back(artist);
            artist_s = artist_s.substr(pos + 1, artist_s.size() - pos);
            pos = artist_s.find(';');
        }
        artists.push_back(artist_s);

        getline(inFile, album_name, ',');
        if (album_name.empty()) {
            getline(inFile, junk);
            continue;
        }

        // Special Case: "Song, Vol. 4" or "Song 1,000"
        getline(inFile, track_name, ',');
        if (track_name.empty()) {
            getline(inFile, junk);
            continue;
        }
        regex digits("^(?!.*\\d{4}).*000.*$"), number("^[0-9]{1,3}$"), milliseconds("^[0-9]{4,}$"),
            parentheses("\\([^)]*$"), ending("\\)");
        // Exclusive Or
        while ((track_name[0] == ' ') !=
            (isdigit(album_name[album_name.length() - 1]) && regex_search(track_name, digits))) {
            album_name += "," + track_name;
            getline(inFile, track_name, ',');
        }

        string token;
        // Has '(' But Not ')'
        if (regex_search(track_name, parentheses)) {
            while (getline(inFile, token, ',')) {
                track_name += "," + token;
                if (regex_search(token, ending)) {
                    break;
                }
            }
        }

        getline(inFile, popularity_s, ',');
        while (popularity_s[0] == ' ' || regex_search(popularity_s, digits) || !regex_match(popularity_s, number)) {
            if (regex_match(popularity_s, milliseconds)) {
                break;
            }
            track_name += "," + popularity_s;
            getline(inFile, popularity_s, ',');
        }

        getline(inFile, duration_ms_s, ',');
        if (!regex_match(duration_ms_s, milliseconds)) {
            track_name += popularity_s + duration_ms_s;
            getline(inFile, duration_ms_s, ',');
            while (!regex_match(duration_ms_s, milliseconds)) {
                track_name += duration_ms_s;
                if (!regex_match(duration_ms_s, number)) {
                    break;
                }
                getline(inFile, duration_ms_s, ',');
            }
            getline(inFile, duration_ms_s, ',');
            popularity_s = duration_ms_s;
            getline(inFile, duration_ms_s, ',');
        }
        int popularity = stoi(popularity_s);
        int duration_ms = stoi(duration_ms_s);

        getline(inFile, explicit_s, ',');
        bool explicit_b;
        if (explicit_s == "True") {
            explicit_b = true;
        } else {
            explicit_b = false;
        }

        getline(inFile, danceability_s, ',');
        float danceability = stof(danceability_s);

        getline(inFile, energy_s, ',');
        float energy = stof(energy_s);

        getline(inFile, key_s, ',');
        int key = stoi(key_s);

        getline(inFile, loudness_s, ',');
        float loudness = stof(loudness_s);

        getline(inFile, mode_s, ',');
        bool mode = stoi(mode_s);

        getline(inFile, speechiness_s, ',');
        float speechiness = stof(speechiness_s);

        getline(inFile, acousticness_s, ',');
        float acousticness = stof(acousticness_s);

        getline(inFile, instrumentalness_s, ',');
        float instrumentalness = stof(instrumentalness_s);

        getline(inFile, liveness_s, ',');
        float liveness = stof(liveness_s);

        getline(inFile, valence_s, ',');
        float valence = stof(valence_s);

        getline(inFile, tempo_s, ',');
        float tempo = stof(tempo_s);

        getline(inFile, time_signature_s, ',');
        int time_signature = stoi(time_signature_s);

        getline(inFile, track_genre);

        if (!genre.empty() && genre != track_genre) {
            continue;
        }

        Song *song = new Song(row, track_id, artists, album_name, track_name, popularity,
                              duration_ms, explicit_b, danceability, energy, key, loudness, mode,
                              speechiness, acousticness, instrumentalness, liveness, valence, tempo,
                              time_signature, track_genre);
        song->similarity = *song - types[type];
       songs_map.insert(song->similarity, song);
        // songs_heap.Insert(*song);
    }
    inFile.close();
   songs_map.printInorder(10);
//    cout << "\n...\n" << endl;
//     songs_heap.printSongs(10);

    return 0;
}
