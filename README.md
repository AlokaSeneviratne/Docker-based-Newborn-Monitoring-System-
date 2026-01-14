# Docker-Based Newborn Monitoring System

This project is focused on creating Newborn Monitoring System in order to monitor the vitals of a newborn remotely and provide real-time updates of the vitals of the newborn. It can be defined as a software-defined IoT health monitoring system. The Monitoring System consists of the following components and services. 

  → Edge device collects vitals   
  → FastAPI makes up the backend, which stores data in a database & serves data  
  → Dashboard visualises live readings  
  → Deployed on OpenShift/Kubernetes  

## System Architecture

This system consists of the following components and micro-services which have been visualized in the following slide.
  → Edge sensor device  
  → Backend API (FastAPI)  
  → MySQL database  
  → Frontend dashboard (Nginx + HTML/JS)  
  → Kubernetes Deployments, Services, Routes  
<img width="1878" height="382" alt="image" src="https://github.com/user-attachments/assets/7a23c7c7-a842-4503-a33a-bceef2b73301" />











