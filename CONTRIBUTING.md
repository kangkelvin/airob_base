# Guide on how to contribute to this repo
`git status` is your best friend

## Steps for making code change
1. `git branch -b <your-branch>` 
for example: `git branch -b enh/kelvin/add_human_detection`
2. `git status` and check that you are in the right branch
3. Make your edit
4. `git branch --set-upstream-to=origin/master <your-branch>` for example `git branch --set-upstream-to=origin/master enh/kelvin/add_human_detection`
5. `git status` and you should see your changes in red color
6. `git add -A` to add the changes, but I suggest you use VS code function to individually select the changes you want
7. `git status` and you should see your changes in green color
8. `git commit` and write a thoughtful commit msg
   
   for example:
   ```
    #4 add human detection algo and ROS nodes

    -Add human detection algo class that takes in RGB img and output BBoxes
    -Add ROS node wrapper around the algo class that sub to ROS img and Pub ROS bounding_box.msg
    -Add unit test for the algo class 
   ```
9. `git commit --amend` if you need to change your latest commit msg
10. `git log` to see that your commit is added, press `q` to exit the log
11. `git pull origin master --rebase` this will pull the latest changes in master and apply it to your branch, but your latest commit is on top of the latest master changes
12. `git log` again to see that your commit is still at the top of any new changes
13. Remember to run all your test first! including the auto-formatter
14. `git push origin <your-branch>` for example `git push origin enh/kelvin/add_human_detection`
15. You should see a message in the console like this: `remote: Create a pull request for '<your-branch<' on GitHub by visiting:` and a url link to the pull request
16. Fill out the PR form properly, including the PR name
17. It is unlikely that your PR is acceptable the first time, so please see below on how to make changes to your PR


## Making changes to your PR
1. Make your changes and don't forget to rerun auto-formatter and run your tests
2. `git add -A` to add the changes, but I suggest you use VS code function to individually select the changes you want
3. `git commit` and this time you can write smth short because we will rebase it soon, for example `fix bug`
4. `git log` and you should see your latest commit on top of your previous commit
5. `git rebase -i` and this will bring you to interactive session to squash your commits together. For example you will see smth like this:

    ```
    pick 4def5bb your original commit msg
    pick 9f93f95 fix bug

    # Rebase dccd638..9f93f95 onto dccd638 (2 commands)
    #
    ```

    Then use your editor and change all the commits after your first commit msg from `pick` to `f`. So it will look like this:

    ```
    pick 4def5bb your original commit msg
    f 9f93f95 fix bug

    # Rebase dccd638..9f93f95 onto dccd638 (2 commands)
    #
    ```
    Remember to save

6. `git log` one more time and you should only see your original commit msg
7. `git commit --amend` in case you want to edit the commit msg to reflect new changes
8. Don't forget to auto-format and run tests
9.  `git push origin <your-branch> -f` NOTICE THE EXTRA FLAG!! No url will show up this time, go to github to see your latest change

## After your PR is merged to master
1. Congratulation, grab a bottle of beer
2. `git checkout master` go back to master branch
3. `git branch -D <your-branch>` to delete your branch, make sure there's no unsaved unchages do you do not want to lose

## Useful Git tools you need to know

### git rm

### git stash

### git reset