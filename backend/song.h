//
// Variable Descriptions Available Here: https://www.kaggle.com/datasets/maharshipandya/-spotify-tracks-dataset
//
#include <iostream>
#pragma once
using namespace std;

#ifndef PROJECT_3_MBTI_MELODY_SONG_H
#define PROJECT_3_MBTI_MELODY_SONG_H

struct Song {
    int row, popularity, duration_ms, key, time_signature;
    string track_id, album_name, track_name, track_genre;
    vector<string> artist = {};
    bool explicit_b, mode;
    float danceability, energy, loudness, speechiness, acousticness, instrumentalness, liveness, valence, tempo;
    double similarity = 0;

    // Constructs Default
    Song() {
        this->row = 0;
        this->track_id = "";
        this->album_name = "";
        this->track_name = "";
        this->popularity = 0;
        this->duration_ms = 0;
        this->explicit_b = false;
        this->danceability = 0.0;
        this->energy = 0.0;
        this->key = 0;
        this->loudness = 0.0;
        this->mode = false;
        this->speechiness = 0.0;
        this->acousticness = 0.0;
        this->instrumentalness = 0.0;
        this->liveness = 0.0;
        this->valence = 0.0;
        this->tempo = 0.0;
        this->time_signature = 0;
        this->track_genre = "";
    }

    // Construct Personality Type
    Song(float danceability, float energy, float loudness,
         float speechiness, float acousticness, float instrumentalness,
         float liveness, float valence, float tempo) {
        this->row = 0;
        this->track_id = "";
        this->album_name = "";
        this->track_name = "";
        this->popularity = 0;
        this->duration_ms = 0;
        this->explicit_b = false;
        this->danceability = danceability;
        this->energy = energy;
        this->key = 0;
        this->loudness = loudness;
        this->mode = false;
        this->speechiness = speechiness;
        this->acousticness = acousticness;
        this->instrumentalness = instrumentalness;
        this->liveness = liveness;
        this->valence = valence;
        this->tempo = tempo;
        this->time_signature = 0;
        this->track_genre = "";
    }

    // Constructs Parameterized
    Song(int row, string& track_id, vector<string>& artist, string& album_name,
         string& track_name, int popularity, int duration_ms, bool explicit_b,
         float danceability, float energy, int key, float loudness, bool mode,
         float speechiness, float acousticness, float instrumentalness,
         float liveness, float valence, float tempo, int time_signature,
         string& track_genre) {
        this->row = row;
        this->track_id = track_id;
        this->artist = artist;
        this->album_name = album_name;
        this->track_name = track_name;
        this->popularity = popularity;
        this->duration_ms = duration_ms;
        this->explicit_b = explicit_b;
        // 0.0 (Low) to 1.0 (High)
        this->danceability = danceability;
        // 0.0 (Low) to 1.0 (High)
        this->energy = energy;
        this->key = key;
        // [dB]
        this->loudness = loudness;
        this->mode = mode;
        // 0.0 (Low) to 1.0 (High)
        this->speechiness = speechiness;
        // 0.0 (Low) to 1.0 (High)
        this->acousticness = acousticness;
        // 0.0 (Low) to 1.0 (High)
        this->instrumentalness = instrumentalness;
        // 0.0 (Low) to 1.0 (High)
        this->liveness = liveness;
        // 0.0 (Low) to 1.0 (High)
        this->valence = valence;
        // [BPM] (Beats per Minute)
        this->tempo = tempo;
        this->time_signature = time_signature;
        this->track_genre = track_genre;
    }

    void display() const {
        cout << "Row: " << row << endl;
        cout << "Track ID: " << track_id << endl;
        cout << "Artist(s): " << artist[0];
        for (int i = 1; i < artist.size(); i++) {
            cout << ", " << artist[i];
        }
        cout << "\nAlbum Name: " << album_name << endl;
        cout << "Track Name: " << track_name << endl;
        cout << "Popularity: " << popularity << endl;
        cout << "Duration [ms]: " << duration_ms << endl;
        cout << "Explicit: " << explicit_b << endl;
        cout << "Danceability: " << danceability << endl;
        cout << "Energy: " << energy << endl;
        cout << "Key: " << key << endl;
        cout << "Loudness: " << loudness << endl;
        cout << "Mode: " << mode << endl;
        cout << "Speechiness: " << speechiness << endl;
        cout << "Acoustiness: " << acousticness << endl;
        cout << "Instrumentalness: " << instrumentalness << endl;
        cout << "Liveness: " << liveness << endl;
        cout << "Valence: " << valence << endl;
        cout << "Tempo: " << tempo << endl;
        cout << "Time Signature: " << time_signature << endl;
        cout << "Track Genre: " << track_genre << endl;
        cout << "Similarity: " << similarity << endl << endl;
    }

    void displayShort() const {
        cout << track_name << " by " << artist[0];
        for (int i = 1; i < artist.size(); i++) {
            cout << ", " << artist[i];
        }
     }

    // Calculates Similarity Score
    double operator-(Song& other) {
        similarity += abs(danceability - other.danceability);
        similarity += abs(energy - other.energy);
        // Took Max and Min Values from .csv
        double loudnessNorm = (loudness + 49.531) / (4.532 + 49.531), otherLoudness = (other.loudness + 49.531) / (4.532 + 49.531);
        similarity += abs(loudnessNorm - otherLoudness);
        similarity += abs(speechiness - other.speechiness);
        similarity += abs(acousticness - other.acousticness);
        similarity += abs(instrumentalness - other.instrumentalness);
        similarity += abs(liveness - other.liveness);
        similarity += abs(valence - other.valence);
        // Normalized the Values
        double tempoNormal = tempo / 243.372, otherTempo = other.tempo / 243.372;
        similarity += abs(tempoNormal - otherTempo);
        return similarity;
    }
};

#endif //PROJECT_3_MBTI_MELODY_SONG_H
