<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better, please fork the repo and create a pull request or simply open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
-->





<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/othneildrew/Best-README-Template">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Best-README-Template</h3>

  <p align="center">
    An awesome README template to jumpstart your projects!
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Report Bug</a>
    ·
    <a href="https://github.com/othneildrew/Best-README-Template/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://example.com)

The goals of this sumbission are:
* Showcase the Java Spring 2 CRUD fully scalable microservice
* Attach a basic UI written in Angular 8 with my modifications to fit my backend model (https://www.devglan.com/spring-boot/spring-boot-angular-example);
* Deploy K8S(GKE) in the GCP in order to deploy my components
* Deploy PostgreSQL DB in GCP as a VM
* Provide a fully workable CI/CD pipeline for backend microservice
* Show a working flow in the cloud

I will try to keep this README small and simple. I will not go over the Java code or structure here as it is very simple and follows standards established in the community for simple CRUD REST component. Will only point out some points which I find interesting.

### Built With
Techs/Tools used in the showcase.
* [Java Spring 2](https://getbootstrap.com)
* [Angular 8](https://jquery.com)
* [GitLab CI/CD](https://laravel.com)
* [GCP, GKE, GCR](https://laravel.com)
* [PostgreSQL](https://laravel.com)

<!-- ARCHITECTURAL DECISIONS -->
## Architectural decisions 

I have choosen to redesign the following project
Here is the list of desinsions I took while redesigning:
1. Use Java Spring 2.1.x with Gradle 5.x. I am very familiar with Java and I like Gradle more than Maven. This speeds up my development speed
2. Used MVC concept to separate the original monolith into modularized, loosely coupled component.
3. Switched MySQL to PostgreSQL for database. If will need to scale DB in the cloud, can use CrunchyDB for this purpose which works with Postgresql.

<!-- BACKEND OVERVIEW -->
## Bakcend Overview 

* application.properties
I have used two properties files to be used in local deployment and in cluster(k8s) environment.
In the application-clustered.properties you an see that the 'prod' db url and auth information is not present. The URL and DB credentials are supplied
to the app inside the k8s environment based on when the app is running. It is achieved by the use of the K8S ConfigMaps. The URL name resolution is happening inside the cluster by the k8s DNS internal services resolution.

* Dockerfile
Provided the Dockerfile for the application in order for it to be portable and run on K8S or any machine that has Docker.

* K8S ConfigMaps
I have build the component with the idea that the configuration has to be provided by the environment and not by the developer in the code. Basically, the component will behave in the expected way based on the environment it runs on. So, the developer should not be concerned about configs as much.
I have used K8S ConfigMap to provide configuration to the Java in runtime.

## To improve for next iteration

* Test code coverage and quality
I have provided some very basic unit testing and one integration test to show that I am familiar with them)) For the next iteration, I would increase the number of tests as well as their quality.

* Fields validations
I do not provide custom exception on every generic exception that the code can throw. For example if somebody provides a name field of more than 50 chars, there will be an exception thrown. I do not explicitely handle it, so it will be thrown in the controller. The exception is clear, but if handeled explicetely, will not polute the output and will display a nicer message.

* Exception handling
I have provided one exception hadling with validation on the email field. As stated earlier, exception on field vslidation ccan be one more to include.

* Authontication/Authorization
The token generation should be a part of a separate component.
This one could act based on token decode from the header and act on contextual permission.
This is to implement in the future iteration.

<!-- FRONTEND OVERVIEW -->
## Frontend Overview

There is not much i can say about frontend. It is an Angular 8 implementation that I took from https://www.devglan.com/spring-boot/spring-boot-angular-example

I have removed Login part and modified model to fit my backend implementation. Showcased that I can work in the UI as well up to a certain point)
The UI does not handle exceptions and custom erros thrown from the backend. I have attached it, so you can see the full flow of application from the UI to backend and back. SAll deployed in the cloud.
To access the UI go here:

<!-- CI/CD PIPELINE OVERVIEW -->
## CI/CD Pipiline Overview
I have implemented the full pipeline for the backend component in GitLab CI/CD. It gets triggered on every commit and performs build, test, docker and deploy to K8S. The pipeline is only implemented for the backend component. Frontend is buyilt in docker and pushed to k8s by me manually.
You can acces the pipeline here.

<!-- GCP, GKE, GCR OVERVIEW -->
## GCP, GKE, GCR Overview

In order to show the scalability of my component, i had to deploy k8s cluster in GCP, deploy my backend and frontend in there and confirm the flow works after I scale my component. In order to load balance the traffic, in my service definition for backend I used type LoadBalancer. GKE, then balances the traffic among multiple replicas. I included k8s manifests in the respective projects.

* beyond-backend.yaml
Contains k8s manifest for the backend component

* postgres-external.yaml
Manifest for the communication with the postgres. This points to an external DB. As you can see, if the DB url and/or port changes, we have to change it her instead of changing it in the config of the component. Very portable.

* prod-cm.yaml
This is the config file which should be deployed in the k8s cluster and it contains all the configurstions for the microservices. In my case it contains the url, username and password for the db. In normasl flow this config file is not part of repository and is handeled separately. In this way nobody see sensitive information. Only people who have access to the cluster can see it.




This is an example of how to list things you need to use the software and how to install them.
* npm
```sh
npm install npm@latest -g
```

### Installation

1. Get a free API Key at [https://example.com](https://example.com)
2. Clone the repo
```sh
git clone https:://github.com/your_username_/Project-Name.git
```
3. Install NPM packages
```sh
npm install
```
4. Enter your API in `config.js`
```JS
const API_KEY = 'ENTER YOUR API';
```



<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_



<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/othneildrew/Best-README-Template/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

Your Name - [@your_twitter](https://twitter.com/your_username) - email@example.com

Project Link: [https://github.com/your_username/repo_name](https://github.com/your_username/repo_name)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Img Shields](https://shields.io)
* [Choose an Open Source License](https://choosealicense.com)
* [GitHub Pages](https://pages.github.com)
* [Animate.css](https://daneden.github.io/animate.css)
* [Loaders.css](https://connoratherton.com/loaders)
* [Slick Carousel](https://kenwheeler.github.io/slick)
* [Smooth Scroll](https://github.com/cferdinandi/smooth-scroll)
* [Sticky Kit](http://leafo.net/sticky-kit)
* [JVectorMap](http://jvectormap.com)
* [Font Awesome](https://fontawesome.com)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=flat-square
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=flat-square
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=flat-square
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=flat-square
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=flat-square
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
