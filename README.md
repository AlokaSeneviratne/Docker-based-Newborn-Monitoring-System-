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
  
<img width="480" height="360" alt="image" src="https://github.com/user-attachments/assets/1fd49f3f-1075-4a81-9d47-c3a28460225a" />
  


<img width="940" height="794" alt="image" src="https://github.com/user-attachments/assets/54f5eb0c-aa71-4fbd-9b1f-670962cc11f8" />













