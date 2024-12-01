#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <set>
#include "song.h"
#include "rb_map.h"
using namespace std;

int main() {
    ifstream inFile("dataset.csv");
    if (inFile.is_open()) {
        cout << "Opened!" << endl;
    } else {
        cout << "Not Opened!" << endl;
        return 1;
    }

    string row_s, track_id, artist, album_name, track_name, popularity_s,
            duration_ms_s, explicit_s, danceability_s, energy_s, key_s, loudness_s, mode_s,
            speechiness_s, acousticness_s, instrumentalness_s, liveness_s, valence_s, tempo_s,
            time_signature_s, track_genre, junk;
    // FIXME: Currently, the Key is Row, But It Should Be Similarity Score
    rb_map<int> songs;
    set<string> genres;

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
    try {
        while (!inFile.eof()) {
            getline(inFile, row_s, ',');
            if (row_s.empty()) {
                getline(inFile, junk);
                break;
            }
            int row = stoi(row_s);

            getline(inFile, track_id, ',');

            // Separated by ';'
            getline(inFile, artist, ',');
            if (artist.empty()) {
                getline(inFile, junk);
                continue;
            }

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
                // Special Numeric Track Names
//                if (track_name == "777") {
//                    break;
//                }
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
                // cout << row_s << " : " << duration_ms_s << endl;
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

            Song *song = new Song(row, track_id, artist, album_name, track_name, popularity,
                             duration_ms, explicit_b, danceability, energy, key, loudness, mode,
                             speechiness, acousticness, instrumentalness, liveness, valence, tempo,
                             time_signature, track_genre);
            songs.insert(row, song);

            genres.insert(track_genre);
        }
    }
    catch (...) {
        cout << "Row: " << row_s << endl;
        cout << "Track ID: " << track_id << endl;
        cout << "Artist: " << artist << endl;
        cout << "Album Name: " << album_name << endl;
        cout << "Track Name: " << track_name << endl;
        cout << "Popularity: " << popularity_s << endl;
        cout << "Duration [ms]: " << duration_ms_s << endl;
        cout << "Explicit: " << explicit_s << endl;
        cout << "Danceability: " << danceability_s << endl;
        cout << "Energy: " << energy_s << endl;
        cout << "Key: " << key_s << endl;
        cout << "Loudness: " << loudness_s << endl;
        cout << "Mode: " << mode_s << endl;
        cout << "Speechiness: " << speechiness_s << endl;
        cout << "Acoustiness: " << acousticness_s << endl;
        cout << "Instrumentalness: " << instrumentalness_s << endl;
        cout << "Liveness: " << liveness_s << endl;
        cout << "Valence: " << valence_s << endl;
        cout << "Tempo: " << tempo_s << endl;
        cout << "Time Signature: " << time_signature_s << endl;
        cout << "Track Genre: " << track_genre << endl << endl;
    }
    inFile.close();

    // Print the First 10 Songs Based on Row
    // songs.printLevelOrder(10);

    return 0;
}
