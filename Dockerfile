FROM ubuntu:24.04

# Install dependencies
RUN apt-get update && apt-get upgrade -y
RUN apt-get -y install meson
RUN apt-get -y install libgtk-3-dev
RUN apt-get -y install ninja-build

# Copy the source code
COPY . /app

# Set the working directory
WORKDIR /app

# Build the application
RUN meson builddir
RUN ninja -C builddir

# Set GTK_THEME to Adwaita:dark
ENV GTK_THEME=Adwaita:dark

# Run the application
CMD ["./builddir/work_01-TAD-Lista"]
