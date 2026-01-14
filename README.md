# Docker-Based Newborn Monitoring System

This project is focused on creating Newborn Monitoring System in order to monitor the vitals of a newborn remotely and provide real-time updates of the vitals of the newborn. It can be defined as a software-defined IoT health monitoring system. The Monitoring System consists of the following components and services. 

  → Edge device collects vitals   
  → FastAPI makes up the backend, which stores data in a database & serves data  
  → Dashboard visualises live readings  
  → Deployed on OpenShift/Kubernetes  

## System Architecture

This system consists of the following components and micro-services which have been visualized in the following images.
  → Edge sensor device  
  → Backend API (FastAPI)  
  → MySQL database  
  → Frontend dashboard (Nginx + HTML/JS)  
  → Kubernetes Deployments, Services, Routes  
 

The image below shows the system architecture in the form of a block diagram

             
  
<img width="600" height="480" alt="image" src="https://github.com/user-attachments/assets/54f5eb0c-aa71-4fbd-9b1f-670962cc11f8" />


## Edge sensor device 

  → Measures HR, SpO₂, temperature, motion  
  → Packages data as JSON   
  → Sends periodic POST /api/data requests  
  → Acts as the system’s data producer  



 <img width="595" height="446" alt="image" src="https://github.com/user-attachments/assets/418bb337-e6d5-4bbe-a19b-f170aa7a32aa" />


## Backend Function

  → Receives Sensor Data   
  → Validate via Pydantic  
  → Provides REST API calls  
  → Handles CORS for dashboard access  


  <img width="868" height="732" alt="image" src="https://github.com/user-attachments/assets/e0775413-a625-4e4d-a2bb-171d55d28017" />

  

## Frontend Dashboard

  → Served by Nginx on OpenShift    
  → HTML + JS + Chart.js    
  → Fetches /api/latest every 2 seconds    
  → Display Metrc Cards and live graphs  



  <img width="491" height="536" alt="image" src="https://github.com/user-attachments/assets/826c0fed-c0ef-40dd-93d3-4d41675ae6ff" />


<img width="566" height="591" alt="image" src="https://github.com/user-attachments/assets/356e92a0-923c-4512-81c1-7925d6e84964" />
















