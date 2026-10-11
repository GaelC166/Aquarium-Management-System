

## How to use

1. Select **1** to add fish to the holding tank.
2. Select **2** to inspect the three tanks.
3. Select **3** to move fish between tanks. The special tank accepts only **S** and **M** fish.
4. Select **4** to remove some fish, or **5** to remove a whole species from the holding tank.
5. Select **6** to see estimated daily feeding costs or **7** to change prices.

The program starts with an empty holding tank (capacity 100), general tank (capacity 80), and special tank (capacity 40, maximum fish size M).

## Feeding-cost assumption

The project report specifies food prices per pound but not a daily food quantity. This reconstruction **assumes each fish eats 0.1 pound per day** for the cost estimate. The assumption can be changed in `Tank::printFoodCost()`.

Default food prices:

| Tank | Meat ($/lb) | Plants ($/lb) |
| --- | ---: | ---: |
| Holding | 4.50 | 2.00 |
| General | 5.00 | 2.50 |
| Special | 5.50 | 2.50 |


## Limitations

- Fish populations are kept in memory and are not saved after exit.
- Tank capacity is counted by number of fish, not physical volume.
- Fish records merge only when name, size, diet, and description match.
- The menu uses fixed tanks to keep the project approachable.
