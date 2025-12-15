# enseash (Abdellahi CHEIKH SIDIYA & Salma REGRAG)
Micro Shell

Objectif du TP : L’objectif du TP est de développer un micro-shell capable d’exécuter des commandes et d’afficher leurs codes de sortie ainsi que leurs temps d’exécution.

1) Affichage d’un message d’accueil :

Dans cette première étape du micro-shell ENSEASH, nous affichons simplement un message d’accueil suivi d’un prompt minimal. Le texte est envoyé sur la sortie standard avec write(STDOUT_FILENO, mess_prompt, strlen(mess_prompt));, conformément aux consignes, et sa longueur est déterminée avec strlen().

2) Exécution de la commande saisie :

a) Pour lire la commande saisie on utilise la ligne de commande suivante : read(STDIN_FILENO, cmd, BUFSIZE).

b) Dans cette étape, le micro-shell ENSEASH affiche d’abord le message d’accueil avec write(), puis entre dans une boucle interactive (REPL). À chaque itération, il lit la commande saisie par l’utilisateur avec read(), crée un processus enfant (pid= 0) avec fork(), et exécute la commande dans ce processus enfant grâce à execlp(). Si l’exécution échoue, perror() affiche un message d’erreur (pid= -1). Le processus parent attend la fin de l’enfant avec wait() avant de réafficher le prompt avec write(), permettant ainsi à l’utilisateur de saisir une nouvelle commande et de rester dans le shell de manière interactive.

c) Cette ligne dans la boucle infinie nous permet de retourner à la ligne suivante après l'exécution d’une commande.
   write(STDOUT_FILENO, "% ", strlen("% "));


3) Gestion de la sortie du shell :
   Le micro-shell ENSEASH gère la sortie à la fois avec la commande exit et avec <Ctrl>+D. Pour exit, on compare la commande saisie à la chaîne "exit" avec     strcmp(), qui    renvoie 0 si les deux chaînes sont identiques. L’exécution de cette vérification se fait avant le fork(), afin d’éviter la création d’un       nouveau processus inutile.     Pour Ctrl +D, qui envoie un signal End Of File (EOF), read() retourne 0, ce qui permet de détecter qu’il n’y a plus rien à        lire. La condition || n == 0 permet donc    de fermer correctement le shell dans ce cas.

4) Cette fonctionnalité permet d’afficher, dans le prompt du shell, le code de retour de la dernière commande exécutée ou le signal qui a provoqué son arrêt.
   Après l’exécution d’une commande, le processus père récupère le statut de fin du processus fils à l’aide de l’appel système wait().
   Si la commande se termine normalement, le code de sortie est affiché sous la forme [exit:code].
   Si la commande est interrompue par un signal, le numéro du signal est affiché sous la forme [sign:num].
   Ce mécanisme permet à l’utilisateur de connaître immédiatement le résultat de la commande précédente.

5) Le shell mesure le temps d’exécution de chaque commande à l’aide de l’appel système clock_gettime() avec l’horloge CLOCK_MONOTONIC.
   Un timestamp est enregistré juste avant la création du processus fils, puis un second après la fin de son exécution.
   La différence entre ces deux instants permet de calculer le temps d’exécution en millisecondes.
   Ce temps est ensuite affiché dans le prompt, en complément du code de retour ou du signal, sous la forme [exit:code|Xms] ou [sign:num|Xms].


