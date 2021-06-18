
<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/neurovisionhub/dft-running-time-prediction">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">A DFT-Based Running Time Prediction Algorithm for
Web Queries</h3>

  <p align="center">
    Oscar Rojas, Veronica Gil-Costa and Mauricio Marín
    <br />
    <a href="https://github.com/neurovisionhub/dft-running-time-prediction"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/neurovisionhub/dft-running-time-prediction/tree/master/train-test">View train-test</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
 <!--   <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>-->
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

Series of machine learning experiments and data mining tests for the evaluation of query execution time predictors in web search engines.

Techniques to predict the processing time of document ranking algorithms are compared, where predicting performance allows to allocate resources efficiently in web search engines.


### run With

* open file ./test.m and run with matlab 

<!-- GETTING STARTED -->
## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites
matlab 2014 or higher

* https://la.mathworks.com/products/matlab.html 

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/neurovisionhub/dft-running-time-prediction.git
   ```
2. cd directory
   ```sh 
   cd ./dft-running-time-prediction
   ```



<!-- USAGE EXAMPLES -->
## Usage


In root folder, edit "test.m" to run predefined experiments (**'simple','parallel','all','cross-fold','visual-map','block'**), where 'configdefault.m' loads the experimental data.



   ```sh
clear
addpath(genpath('./'))
configdefault;
cd train-test\

% config examples
% Simple test Baseline vs DFT-Based 
% Selecction data-web
[data_simple,labelsD,labelsA]= main('simple');
data = [labelsD;labelsA;data_simple];
data = [["","", "Baseline","DFT-Based"]',data];
T = table(data)

% runtime-parallel prediction
%[data_parallel,~]= main('parallel');
.
.

   ```


main.m is a routine that receives as input a label (that describes the experiment) and outputs an array or tables.

Matrix Data

   ```sh
   data_simple =

    0.9636    0.0219    0.8906    0.0356    0.9015    0.0490    0.9309    0.0330
    0.9688    0.0203    0.9279    0.0290    0.9235    0.0432    0.9324    0.0325

Table

                                       data                                    
    ___________________________________________________________________________

    ""             "W.CW.BM25"    "W.CW.BM25"    "BMW.CW.BM25"    "BMW.CW.BM25"
    ""             "PCr"          "RMSE"         "PCr"            "RMSE"       
    "Baseline"     "0.96363"      "0.021945"     "0.90145"        "0.049025"   
    "DFT-Based"    "0.96875"      "0.020327"     "0.92353"        "0.043161"
   ```





<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.



<!-- CONTACT -->
## Contact

O. Rojas - [orojas.cl](https://orojas.cl) - oscar.rojas.d@usach.cl

Project Link: [https://github.com/neurovisionhub/dft-running-time-prediction](https://github.com/neurovisionhub/dft-running-time-prediction)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements

* []()
* []()
* []()

<!-- 
<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.


<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/github_username/repo.svg?style=for-the-badge
[forks-url]: https://github.com/github_username/repo/network/members
[stars-shield]: https://img.shields.io/github/stars/github_username/repo.svg?style=for-the-badge
[stars-url]: https://github.com/github_username/repo/stargazers
[issues-shield]: https://img.shields.io/github/issues/github_username/repo.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo/issues
[license-shield]: https://img.shields.io/github/license/github_username/repo.svg?style=for-the-badge
[license-url]: https://github.com/github_username/repo/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/github_username
