FROM nginx:alpine

USER root

# allow OpenShift non-root UIDs to access these directories
RUN mkdir -p /var/cache/nginx \
    && chmod -R 777 /var/cache/nginx \
    && chmod -R 777 /var/run \
    && chmod -R 777 /var/log/nginx

COPY index.html /usr/share/nginx/html/index.html
COPY nginx.conf /etc/nginx/nginx.conf

# run as OpenShift-compatible UID
USER 1001
