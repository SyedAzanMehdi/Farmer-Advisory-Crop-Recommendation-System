# Farmer Advisory and Crop Recommendation System

This project is a C++–based command-line application designed to assist farmers in making informed decisions about crop selection. The system analyzes environmental factors such as soil type, temperature, rainfall, humidity, and season to generate intelligent crop recommendations. It uses data structures, algorithms, and persistent storage to provide a complete advisory workflow.

# Key Features
# Intelligent Crop Recommendation

Suggests crops based on temperature, rainfall, soil type, and season

Utilizes a scoring algorithm to determine suitability

Provides estimated yield predictions

# Regional Climate Modelling

Stores climate information for each region

Supports comparison between climate-similar regions

Enables regional environmental analysis

# Crop Management (BST)

Binary Search Tree for storing and retrieving crop information

Add, search, browse, and view detailed crop profiles

Supports maturity period, fertilizer requirement, and yield data

# Soil-Based Suggestions

Maps soil types to suitable crops

Offers soil-specific insights

# Data Persistence

All data is stored in files, which are automatically created and updated:

crops.dat

regions.dat

soil_mapping.dat

recommendations.log

# System Architecture
# Component          	Description
CropBST------------   Stores all crops using a binary search tree
RegionGraph------------Graph-based climate data and region connectivity
RecommendationEngine------------Computes suitability scores for crops
FileHandler------------	 Handles all saving and loading operations
DataManager------------	Manages initialization and CRUD operations
DisplayManager------------	Responsible for all formatted output
InputValidator------------	Ensures safe and reliable user input
AnimatedText------------	Provides animated introductory text
# Technical Stack

C++17

Standard Template Library (STL)

OOP principles

Graph data structures

Binary Search Trees

File I/O (persistent storage)

# Build and Run Instructions
# Compile
g++ main.cpp -o advisor

# Execute
./advisor

# Project Structure
/Farmer-Advisory-System
│── main.cpp
│── README.md
│── crops.dat
│── regions.dat
│── soil_mapping.dat
│── recommendations.log
│── /docs (optional)

# Recommendation Algorithm (Overview)

The suitability score for each crop is calculated using the following criteria:

Temperature suitability

Soil type compatibility

Rainfall and water requirement matching

Seasonal appropriateness

Yield potential

Crops with higher scores are considered more suitable and are presented at the top of the recommendation list.
