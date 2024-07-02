#!/bin/bash

install_dependency() {
  local command_name="$1"
  local package_name="$2"

  if ! command -v "$command_name" &>/dev/null; then
    echo "$package_name não está instalado."
    read -p "Deseja instalar $package_name? (S/N): " resposta
    if [ "$resposta" == "S" ] || [ "$resposta" == "s" ]; then
      sudo apt-get install "$package_name" -y
    else
      echo "Instalação de $package_name cancelada."
    fi
  else
    echo "$package_name já está instalado."
  fi
}

if [ "$(uname -s)" != "Linux" ]; then
  echo "Este script suporta apenas sistemas operacionais Linux."
  exit 1
fi

echo "Sistema Operacional Linux detectado."
echo "Escolha o método de execução:"
echo "1 - Executar localmente"
echo "2 - Executar usando Docker"
read -p "Digite o número correspondente à opção desejada: " opcao_execucao

case "$opcao_execucao" in
1)
  echo "Executando localmente..."

  echo "Atualizando pacotes no Linux..."
  sudo apt-get update

  install_dependency "meson" "meson"
  install_dependency "ninja" "ninja-build"
  install_dependency "pkg-config" "pkg-config"

  if ! pkg-config --exists gtk+-3.0; then
    echo "GTK não está instalado."
    read -p "Deseja instalar o GTK? (S/N): " resposta
    if [ "$resposta" == "S" ] || [ "$resposta" == "s" ]; then
      sudo apt-get install libgtk-3-dev
    else
      echo "Instalação do GTK cancelada."
    fi
  else
    echo "GTK já está instalado."
  fi

  if [ ! -d builddir ]; then
    echo "Criando setup builddir..."
    meson setup builddir
  else
    echo "Atualizando setup..."
    meson compile -C builddir
  fi

  echo "Compilando o projeto..."
  ninja -C builddir

  echo "Executando o projeto..."
  ./builddir/main
  ;;
2)
  echo "Executando usando Docker..."

  install_dependency "docker" "docker"

  sudo docker build -t work_01-tad-lista .
  sudo xhost +local:root
  sudo docker run -it --rm --env DISPLAY=$DISPLAY --env XAUTHORITY=$XAUTHORITY --volume /tmp/.X11-unix:/tmp/.X11-unix work_01-tad-lista
  sudo xhost -local:root
  ;;
*)
  echo "Opção inválida. Saindo..."
  ;;
esac
