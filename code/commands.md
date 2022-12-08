# development

## start databse docker

```
docker-compose run --service-ports -d mongodb
```

## start server manually

```
# in server folder
flask run --host 0.0.0.0
```

## stop dockers

```
docker-compose down
```

# deployment

## build containers

```
docker-compose build
```

## start containers

```
docker-compose up
```
