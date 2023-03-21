# Solar cooker

This repository contains the [arduino code](arduino) and [wiring diagrams](kicad) for the various components.

# Git

## Commits

If you forgot something in the previous commit and don't want to create a new commit, you can use the following.

Use the `--no-edit` flag to keep the same commit message.

```bash
git commit --ammend --no-edit
```

## Branches

Make sure to always work on your own branch!

### Show all branches

```bash
git branch --all
```

### Switch to specific branch

If the local branch does not already exist, one is created. Never checkout `remotes/origin/<branch-name>` just use `<branch-name>`.

```bash
git checkout <branch-name>
```

## Updating your local branch

### Stash

If you have made local adjustments but are not yet ready for a commit, you can save it with a stash.

```bash
git stash -u
```

To retrieve the stash use

```bash
git stash pop
```

### Pull

Usually a git pull is sufficient.

```bash
git pull
```

### Reset

If the last commits have multiple authors, the history may have been overwritten and you will have to do a reset.

```bash
git reset --hard origin/<your-branch-name>
```

### Don't lose data while doing a reset

If you don't want to accidentally lose changes, make a copy of the repository's current status.

```bash
git branch <backup-branch-name>
```

### Rebase

If your own branch is a few commits behind, you can pull in updates from another branch.

```bash
git rebase <up-to-date-branch>
```
