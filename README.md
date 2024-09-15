<a id="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<h1 align="center">RediX</h3>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#RediX-data-structures">RediX Data Structrues</a></li>
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
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project
RediX is an in-memory database, meaning it is fast but volatile. RediX is great for remote caching due to its high performance, low-latency access, and support for various data structures.

### RediX supports the following data structures:

* Strings
* Lists
* Sorted sets
* More to come

### Built With

* C++ 20
* Google Test Framework

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started
### Prerequisites
Please run on a Linux OS, with the g++ compiler and CMake.
### Installation
1. Clone the repo
   ```sh
   git clone https://github.com/eggrollius/RediX.git
   cd RediX
   ```
2. build with CMake
   ```sh
   cmake -S . -B build
   cmake --build build
   ```
3. Run the server optionally specifying the ip and port
    ```sh
    ./redix
    ```
4. In a new terminal, run the RediX cli
   ```sh
   ./redixcli
   ```
5. Test out some commands!
    ``` sh
    127.0.0.1:6389> SET key value
    OK
    127.0.0.1:6389> GET key
    value
    ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/eggrollius/RediX/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=eggrollius/RediX" alt="contrib.rocks image" />
</a>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Ethan Huynh - ethanhuynh1124@gmail.com

Project Link: [https://github.com/eggrollius/RediX](https://github.com/eggrollius/RediX)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/eggrollius/RediX.svg?style=for-the-badge
[contributors-url]: https://github.com/eggrollius/RediX/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/eggrollius/RediX.svg?style=for-the-badge
[forks-url]: https://github.com/eggrollius/RediX/network/members
[stars-shield]: https://img.shields.io/github/stars/eggrollius/RediX.svg?style=for-the-badge
[stars-url]: https://github.com/eggrollius/RediX/stargazers
[issues-shield]: https://img.shields.io/github/issues/eggrollius/RediX.svg?style=for-the-badge
[issues-url]: https://github.com/eggrollius/RediX/issues
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/ethanthuynh
[product-screenshot]: images/screenshot.png
