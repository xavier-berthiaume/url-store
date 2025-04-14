# Categora-extension

**Application frontend qui permet à un utilisateur de sauvegarder ses liens web**

## Dépendences

Présentement Categora-extension n'a pas de dépendences. Cette application à été écrit exclusivement en HTML, CSS et Javascript.

## Navigateurs Web Supportés

- Firefox
- Google Chrome

## Installation et Debug

- Firefox

1. Accéder à l'addresse `about:debugging`
2. Sous la section `Extensions temporaires` appuyer sur le bouton `Charger un module <...>`
3. Trouver le fichier manifest.json de Categora-extension et le sélectionner

- Google Chrome

1. Accéder à l'addresse `chrome://extensions`
2. Activer le mode `Mode dévéloppeur`
3. Chargez l'extension

## Dévéloppment

Pour aider avec le dévéloppement, il faut simplement installer le module npm watch pour autocharger les changements apportées au code.

```bash
npm install
npm run watch
```
