# Instructions for the Final Project

Now, based on the knowledge you have gained through the exercises, you need to have your own project that employs what you have learnt so far. In other words, you need to make a dockerized application that can interact with your resources in Rahti. Your application can be anything as long as it can be dockerized and can be deployed in Rahti (either its backend, or frontend, or both parts, or some parts, or whatever...).

Things to consider when preparing the final project:

- Your application does not need to be super fancy and beautiful. In other words, we evaluate the interaction between microservices both in your local machine and in the Rahti and how they communicate with each other, not the shininess of your frontend. Focus on the backend.
- You don't need to build your application from scratch. You can use open source applications and break them into multiple microservices that interact with each other both locally and via Rahti resources.
- Your own application does not need to use advanced algorithms and AI technologies to get the highest score. The most important thing is that your application is not monolithic, and you have multiple microservices that interact with each other (similar to the exercises that you have done so far).

## Deliverables

- Source code and other material in the [Final-Project/](.) directory
- Github/Gitlab repo link ([open-source.url](./open-source.url)) if you decide to use open source projects (don't copy all of the open-source project here; just provide the link to the open-source project)
- Provide a link ([video-demonstration file](video-demonstration.url)) to a recorded short video (about 10 minutes) with the following conditions:
  - Have your mic and camera on while screen recording your desktop
  - Introduce yourself by stating your full name
  - Explain what is the project all about and what you have done
  - Run the commands step by step, and show that the whole implementation is working seamlessly
  - You could use your office365 (or any other online place like YouTube etc.) for storing the recorded video and share its link in here
- Take a look at the [arch.png](./arch.png) file and replace it with the architecture of your project that shows the general idea of the project and how components interact with each other

Run the following commands in your terminal in the [Final-Project/](.) directory to deliver the final project:

```shell
git add .
git commit -m "Final Project"
git push
```
