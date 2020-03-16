## Ajouter la référence au repo distant
> git remote add origin https://gitlab.com/martinboue/enssat-jee-gestion-de-cong-s

## Récupérer les modifications distantes (à faire une fois à l'initialisation) (attention ça écrase tout)
> git fetch --all

> git reset --hard origin/master 

## Récupérer les modifications de la branche distante dans le cas normal
> git pull

## Changer de branche (l'option -b permet de créer la branche)
> git checkout -b nom_branche

## Ajouter ses modifications à un commit : 
### 1- Regarder les modifications (importants)
> git status
	
#### Ne mettre que les fichiers intéressants (éviter les git add . ou -a)
> git add nom_fichier
	
### 2- Commit ses modifications (Avec un bon message plz)
> git commit -m "Add | Remove | Fix | Update - message" 

## Pousser sur la branche distante qui porte le même nom que c'elle sur laquelle on est en locale 
> git push

## Pousser sur une autre branche que la locale
> git push origin branche_locale:branche_distante

## Merger une branche sur master (à faire depuis master)
> git merge branche_a_merge

## Rebaser sa branche sur une autre branche (depuis la branche que l'on veut rebase)
> git rebase autre_branche

## Récupérer une branche distante et la merge avec la branche actuelle
> git pull origin branche_distante

## Conseils 
### Eviter de travailler sur master directement
### Re-sync sa master régulièrement avec son dépôts distants
### Rebase sa branche de feature sur master quand on commence un nouveau "groupe" de features