FROM php:apache

COPY index.php /var/www/html
COPY apache2.conf /etc/apache2/apache2.conf
# insert a copy of our web-pages
COPY login.php /var/www/html/login.php
COPY task1ref.php /var/www/html/task1ref.php
COPY task2stored.php /var/www/html/task2stored.php
COPY task3dom.php /var/www/html/task3dom.php
COPY gradesPortal.php /var/www/html/gradesPortal.php
COPY GradersPortalTask2.php /var/www/html/GradersPortalTask2.php
COPY studentManagerDOMBASED.php /var/www/html/studentManagerDOMBASED.php
COPY AdminViewComments.php /var/www/html/AdminViewComments.php

# RUN apt install iproute2
RUN docker-php-ext-install mysqli 
RUN apt-get update &&\
    apt-get install -y\
    build-essential \
    vim

EXPOSE 80
