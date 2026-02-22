# Farmer Advisory and Crop Recommendation System

The Farmer Advisory and Crop Recommendation System is a C++17 command-line application designed to help farmers make data-driven crop selection decisions. The system analyzes environmental and agricultural parameters to generate intelligent recommendations using algorithmic scoring models.

This project demonstrates advanced data structure usage, object-oriented design, and persistent data storage for agricultural decision support simulation.

# Core Features
# Intelligent Crop Recommendation

Recommends crops based on:

Temperature

Rainfall

Humidity

Soil type

Season compatibility

Uses a scoring algorithm to rank crop suitability

Provides estimated yield insights based on environmental conditions

# Regional Climate Modeling

Maintains climate data for different regions

Enables comparison between climate profiles

Supports environmental pattern analysis for better recommendations

# Crop Information Management (BST Implementation)

Uses Binary Search Tree (BST) for efficient crop storage and retrieval

# Supports:

Adding new crop records

Searching crop profiles

Browsing detailed crop information

Stores crop attributes such as:

Maturity period

Fertilizer requirements

Yield statistics

# Soil-Based Crop Guidance

Maps soil types to compatible crops

Provides soil-specific agricultural insights

# Data Persistence

All system data is stored using file handling for long-term persistence:

crops.dat

regions.dat

soil_mapping.dat

recommendations.log

# System Architecture

CropBST – Manages crop records using Binary Search Tree structure

RegionGraph – Represents regional climate relationships using graph modeling

RecommendationEngine – Computes crop suitability scores

FileHandler – Handles persistent storage operations

DataManager – Controls data initialization and CRUD workflows

DisplayManager – Formats system output for usability

InputValidator – Ensures safe user input processing

AnimatedText – Provides interactive console introduction

# Technical Stack

C++

Standard Template Library (STL)

Object-Oriented Programming principles

Graph and Tree data structures

File-based database simulation

# Recommendation Algorithm Logic

Crop suitability ranking is determined using:

Climate temperature matching

Soil compatibility analysis

Water and rainfall requirement alignment

Seasonal farming suitability

Expected yield optimization

Crops with higher aggregate scores are prioritized in recommendation results.
