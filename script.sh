#!/bin/bash

# Função para verificar e instalar dependências
install_dependency() {
  local command_name="$1"
  local package_name="$2"

  # Verifica se o comando já está instalado
  if ! command -v "$command_name" &>/dev/null; then
    echo "$package_name não está instalado."
    read -p "Deseja instalar $package_name? (S/N): " resposta
    if [ "$resposta" == "S" ] || [ "$resposta" == "s" ]; then
      if [ "$OS" == "Linux" ]; then
        sudo apt-get install "$package_name"
      elif [ "$OS" == "Mac" ]; then
        brew install "$package_name"
      fi
    else
      echo "Instalação de $package_name cancelada."
    fi
  else
    echo "$package_name já está instalado."
  fi
}

# Verificar e atribuir o sistema operacional
if [ "$(uname -s)" == "Linux" ]; then
  OS="Linux"
  echo "Sistema Operacional Linux"
elif [ "$(uname -s)" == "Darwin" ]; then
  OS="Mac"
  echo "Sistema Operacional Mac"
else
  echo "Sistema Operacional não suportado"
  exit 1
fi

echo "Escolha o método de execução:"
echo "1 - Executar localmente"
echo "2 - Executar usando Docker"
read -p "Digite o número correspondente à opção desejada: " opcao_execucao

case "$opcao_execucao" in
1)
  echo "Executando localmente..."

  # Atualizar os repositórios de pacotes
  if [ "$OS" == "Linux" ]; then
    echo "Atualizando pacotes no Linux..."
    sudo apt-get update
  elif [ "$OS" == "Mac" ]; then
    echo "Atualizando pacotes no Mac..."
    brew update
  fi

  # Verificar e instalar as dependências
  install_dependency "meson" "meson"
  install_dependency "ninja" "ninja-build"
  install_dependency "pkg-config" "pkg-config"

  # Verificar se o GTK está instalado
  if ! pkg-config --exists gtk+-3.0; then
    echo "GTK não está instalado."
    read -p "Deseja instalar o GTK? (S/N): " resposta
    if [ "$resposta" == "S" ] || [ "$resposta" == "s" ]; then
      if [ "$OS" == "Linux" ]; then
        sudo apt-get install libgtk-3-dev
      elif [ "$OS" == "Mac" ]; then
        echo "GTK não é suportado no macOS através de brew diretamente. Consulte a documentação oficial para instalação."
        exit 1
      fi
    else
      echo "Instalação do GTK cancelada."
    fi
  else
    echo "GTK já está instalado."
  fi

  # Se não existir a pasta builddir, criar
  if [ ! -d builddir ]; then
    echo "Criando setup builddir..."
    meson setup builddir
  else
    echo "Atualizando setup..."
    meson compile -C builddir
  fi

  # Compilar o projeto
  echo "Compilando o projeto..."
  ninja -C builddir

  # Executar o projeto
  echo "Executando o projeto..."
  ./builddir/work_01-TAD-Lista
  ;;
2)
  echo "Executando usando Docker..."

  # Verificar se o Docker está instalado
  if ! command -v docker &>/dev/null; then
    echo "Docker não está instalado."
    read -p "Deseja instalar o Docker? (S/N): " resposta
    if [ "$resposta" == "S" ] || [ "$resposta" == "s" ]; then
      if [ "$OS" == "Linux" ]; then
        sudo apt-get install docker.io
      elif [ "$OS" == "Mac" ]; then
        brew install docker
      fi
    else
      echo "Instalação do Docker cancelada."
    fi
  else
    echo "Docker já está instalado."
  fi

  sudo docker build -t work_01-tad-lista .
  sudo xhost +local:root
  sudo docker run -it --rm --env DISPLAY=$DISPLAY --env XAUTHORITY=$XAUTHORITY --volume /tmp/.X11-unix:/tmp/.X11-unix work_01-tad-lista
  sudo xhost -local:root
  ;;
*)
  echo "Opção inválida. Saindo..."
  ;;
esac
