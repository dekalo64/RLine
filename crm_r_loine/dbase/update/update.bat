<<<<<<< HEAD
@echo on

cls

@echo ***************
@echo UPDATE DATABASE			   
@echo ***************
@echo.

@echo off

@echo Step 1. Delete old procedure
@echo.
 sqlcmd -S Lena\MSSQLEXPRESS -d VL -U sa -P 111 -i delete.sql 
@echo Delete procedure complete
@echo.

@echo Step 2. Update procedure
@echo.
for %%a in (../scripts/*.sql) do (
 sqlcmd -S Lena\MSSQLEXPRESS -d VL -U sa -P 111 -i ../scripts/%%a
)
@echo Update procedure complete
@echo.

=======
@echo on

cls

@echo ***************
@echo UPDATE DATABASE			   
@echo ***************
@echo.

@echo off

@echo Step 1. Delete old procedure
@echo.
 sqlcmd -S .\MSSQLEXPRESS -d VL -U sa -P 111 -i delete.sql 
@echo Delete procedure complete
@echo.

@echo Step 2. Update procedure
@echo.
for %%a in (../scripts/*.sql) do (
 sqlcmd -S .\MSSQLEXPRESS -d VL -U sa -P 111 -i ../scripts/%%a
)
@echo Update procedure complete
@echo.

>>>>>>> 7d34168ed62c3fc6f2c3e44c48dc517eb050ca29
pause