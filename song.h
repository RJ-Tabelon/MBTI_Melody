#include <iostream>
using namespace std;

// FIXME: Update to Title of Your Project ("Project_3_MBTI_Melody" Was the Name on Local Computer)
#ifndef PROJECT_3_MBTI_MELODY_SONG_H
#define PROJECT_3_MBTI_MELODY_SONG_H

struct Song {
    int row, popularity, duration_ms, key, time_signature;
    string track_id, artist, album_name, track_name, track_genre;
    bool explicit_b, mode;
    float danceability, energy, loudness, speechiness, acousticness, instrumentalness, liveness, valence, tempo;
    Song() {
        this->row = 0;
        this->track_id = "";
        this->artist = "";
        this->album_name = "";
        this->track_name = "";
        this->popularity = 0;
        this->duration_ms = 0;
        this->explicit_b = 0;
        this->danceability = 0.0;
        this->energy = 0.0;
        this->key = 0;
        this->loudness = 0.0;
        this->mode = 0;
        this->speechiness = 0.0;
        this->acousticness = 0.0;
        this->instrumentalness = 0.0;
        this->liveness = 0.0;
        this->valence = 0.0;
        this->tempo = 0.0;
        this->time_signature = 0;
        this->track_genre = "";
    }
    Song(int row, string track_id, string artist, string album_name,
         string track_name, int popularity, int duration_ms, bool explicit_b,
         float danceability, float energy, int key, float loudness, bool mode,
         float speechiness, float acousticness, float instrumentalness,
         float liveness, float valence, float tempo, int time_signature,
         string track_genre) {
        this->row = row;
        this->track_id = track_id;
        this->artist = artist;
        this->album_name = album_name;
        this->track_name = track_name;
        this->popularity = popularity;
        this->duration_ms = duration_ms;
        this->explicit_b = explicit_b;
        this->danceability = danceability;
        this->energy = energy;
        this->key = key;
        this->loudness = loudness;
        this->mode = mode;
        this->speechiness = speechiness;
        this->acousticness = acousticness;
        this->instrumentalness = instrumentalness;
        this->liveness = liveness;
        this->valence = valence;
        this->tempo = tempo;
        this->time_signature = time_signature;
        this->track_genre = track_genre;
    }
    // FIXME: Operator Overload for Similarity Score?
};

#endif //PROJECT_3_MBTI_MELODY_SONG_H
