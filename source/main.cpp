#include "game.h"
#include <stdio.h>
#include <io/pad.h>
#include <sysutil/sysutil.h>

// O PS3 precisa dessa função pra fechar o jogo certinho
// quando aperta o botão PS
static void sysutil_exit_callback(uint64_t status, uint64_t param, void* userdata) {
    (void)param;
    (void)userdata;
    if (status == SYSUTIL_EXIT_GAME) {
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // Registra o callback de sair do jogo
    sysUtilRegisterCallback(0, sysutil_exit_callback, NULL);

    // Cria o jogo
    Game game;

    // Tenta iniciar o SDL e a tela
    if (!game.init()) {
        printf("Erro ao iniciar o jogo!\n");
        return 1;
    }

    // Roda o loop principal (menu, jogando, game over)
    game.run();

    // Fecha tudo limpo
    game.shutdown();

    return 0;
}