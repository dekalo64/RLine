<<<<<<< HEAD
<<<<<<< HEAD
use [VL]

IF OBJECT_ID('[VL].dbo.CopyPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPartnerType]
GO

IF OBJECT_ID('[VL].dbo.CopyPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPosition]
GO

IF OBJECT_ID('[VL].dbo.CopyPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPriorityType]
GO

IF OBJECT_ID('[VL].dbo.CopyStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyStatusType]
GO

IF OBJECT_ID('[VL].dbo.CopyTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyTaskType]
GO

IF OBJECT_ID('[VL].dbo.crm_FindCities', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[crm_FindCities]
GO

IF OBJECT_ID('[VL].dbo.crm_ReadAllCities', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[crm_ReadAllCities]
GO

IF OBJECT_ID('[VL].dbo.DeletePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePartnerType]
GO

IF OBJECT_ID('[VL].dbo.DeletePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePosition]
GO

IF OBJECT_ID('[VL].dbo.DeletePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePriorityType]
GO

IF OBJECT_ID('[VL].dbo.DeleteStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeleteStatusType]
GO

IF OBJECT_ID('[VL].dbo.DeleteTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeleteTaskType]
GO

IF OBJECT_ID('[VL].dbo.FindPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPosition]
GO

IF OBJECT_ID('[VL].dbo.InsertPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPartnerType]
GO

IF OBJECT_ID('[VL].dbo.InsertPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPosition]
GO

IF OBJECT_ID('[VL].dbo.InsertStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertStatusType]
GO

IF OBJECT_ID('[VL].dbo.InsertTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertTaskType]
GO

IF OBJECT_ID('[VL].dbo.InsertPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPosition]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllStatusType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPartnerType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllTaskType]
GO

IF OBJECT_ID('[VL].dbo.ReadCurrentUser', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadCurrentUser]
GO

IF OBJECT_ID('[VL].dbo.UpdatePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePartnerType]
GO

IF OBJECT_ID('[VL].dbo.UpdatePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePosition]
GO

IF OBJECT_ID('[VL].dbo.UpdatePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePriorityType]
GO

IF OBJECT_ID('[VL].dbo.UpdateStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdateStatusType]
GO

IF OBJECT_ID('[VL].dbo.UpdateTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdateTaskType]
GO

IF OBJECT_ID('[VL].dbo.FindPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPartnerType]
GO

IF OBJECT_ID('[VL].dbo.FindPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPriorityType]
GO

IF OBJECT_ID('[VL].dbo.FindStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindStatusType]
GO

IF OBJECT_ID('[VL].dbo.FindTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindTaskType]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePartnerType]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePosition]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadOneStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOneStatusType]
GO

IF OBJECT_ID('[VL].dbo.ReadOneTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOneTaskType]
=======
use [VL]

IF OBJECT_ID('[VL].dbo.CopyPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPartnerType]
GO

IF OBJECT_ID('[VL].dbo.CopyPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPosition]
GO

IF OBJECT_ID('[VL].dbo.CopyPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPriorityType]
GO

IF OBJECT_ID('[VL].dbo.CopyStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyStatusType]
GO

IF OBJECT_ID('[VL].dbo.CopyTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyTaskType]
GO

IF OBJECT_ID('[VL].dbo.crm_FindCities', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[crm_FindCities]
GO

IF OBJECT_ID('[VL].dbo.crm_ReadAllCities', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[crm_ReadAllCities]
GO

IF OBJECT_ID('[VL].dbo.DeletePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePartnerType]
GO

IF OBJECT_ID('[VL].dbo.DeletePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePosition]
GO

IF OBJECT_ID('[VL].dbo.DeletePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePriorityType]
GO

IF OBJECT_ID('[VL].dbo.DeleteStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeleteStatusType]
GO

IF OBJECT_ID('[VL].dbo.DeleteTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeleteTaskType]
GO

IF OBJECT_ID('[VL].dbo.FindPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPosition]
GO

IF OBJECT_ID('[VL].dbo.InsertPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPartnerType]
GO

IF OBJECT_ID('[VL].dbo.InsertPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPosition]
GO

IF OBJECT_ID('[VL].dbo.InsertStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertStatusType]
GO

IF OBJECT_ID('[VL].dbo.InsertTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertTaskType]
GO

IF OBJECT_ID('[VL].dbo.InsertPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPosition]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllStatusType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPartnerType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllTaskType]
GO

IF OBJECT_ID('[VL].dbo.ReadCurrentUser', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadCurrentUser]
GO

IF OBJECT_ID('[VL].dbo.UpdatePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePartnerType]
GO

IF OBJECT_ID('[VL].dbo.UpdatePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePosition]
GO

IF OBJECT_ID('[VL].dbo.UpdatePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePriorityType]
GO

IF OBJECT_ID('[VL].dbo.UpdateStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdateStatusType]
GO

IF OBJECT_ID('[VL].dbo.UpdateTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdateTaskType]
GO

IF OBJECT_ID('[VL].dbo.FindPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPartnerType]
GO

IF OBJECT_ID('[VL].dbo.FindPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPriorityType]
GO

IF OBJECT_ID('[VL].dbo.FindStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindStatusType]
GO

IF OBJECT_ID('[VL].dbo.FindTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindTaskType]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePartnerType]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePosition]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadOneStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOneStatusType]
GO

IF OBJECT_ID('[VL].dbo.ReadOneTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOneTaskType]
>>>>>>> 7d34168ed62c3fc6f2c3e44c48dc517eb050ca29
=======
use [VL]

IF OBJECT_ID('[VL].dbo.CopyPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPartnerType]
GO

IF OBJECT_ID('[VL].dbo.CopyPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPosition]
GO

IF OBJECT_ID('[VL].dbo.CopyPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyPriorityType]
GO

IF OBJECT_ID('[VL].dbo.CopyStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyStatusType]
GO

IF OBJECT_ID('[VL].dbo.CopyTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[CopyTaskType]
GO

IF OBJECT_ID('[VL].dbo.crm_FindCities', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[crm_FindCities]
GO

IF OBJECT_ID('[VL].dbo.crm_ReadAllCities', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[crm_ReadAllCities]
GO

IF OBJECT_ID('[VL].dbo.DeletePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePartnerType]
GO

IF OBJECT_ID('[VL].dbo.DeletePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePosition]
GO

IF OBJECT_ID('[VL].dbo.DeletePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeletePriorityType]
GO

IF OBJECT_ID('[VL].dbo.DeleteStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeleteStatusType]
GO

IF OBJECT_ID('[VL].dbo.DeleteTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[DeleteTaskType]
GO

IF OBJECT_ID('[VL].dbo.FindPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPosition]
GO

IF OBJECT_ID('[VL].dbo.InsertPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPartnerType]
GO

IF OBJECT_ID('[VL].dbo.InsertPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPosition]
GO

IF OBJECT_ID('[VL].dbo.InsertStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertStatusType]
GO

IF OBJECT_ID('[VL].dbo.InsertTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertTaskType]
GO

IF OBJECT_ID('[VL].dbo.InsertPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[InsertPriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPosition]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllStatusType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllPartnerType]
GO

IF OBJECT_ID('[VL].dbo.ReadAllTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadAllTaskType]
GO

IF OBJECT_ID('[VL].dbo.ReadCurrentUser', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadCurrentUser]
GO

IF OBJECT_ID('[VL].dbo.UpdatePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePartnerType]
GO

IF OBJECT_ID('[VL].dbo.UpdatePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePosition]
GO

IF OBJECT_ID('[VL].dbo.UpdatePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdatePriorityType]
GO

IF OBJECT_ID('[VL].dbo.UpdateStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdateStatusType]
GO

IF OBJECT_ID('[VL].dbo.UpdateTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[UpdateTaskType]
GO

IF OBJECT_ID('[VL].dbo.FindPartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPartnerType]
GO

IF OBJECT_ID('[VL].dbo.FindPriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindPriorityType]
GO

IF OBJECT_ID('[VL].dbo.FindStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindStatusType]
GO

IF OBJECT_ID('[VL].dbo.FindTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[FindTaskType]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePartnerType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePartnerType]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePosition', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePosition]
GO

IF OBJECT_ID('[VL].dbo.ReadOnePriorityType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOnePriorityType]
GO

IF OBJECT_ID('[VL].dbo.ReadOneStatusType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOneStatusType]
GO

IF OBJECT_ID('[VL].dbo.ReadOneTaskType', 'P') IS NOT NULL
 DROP PROCEDURE [dbo].[ReadOneTaskType]
>>>>>>> 7d34168ed62c3fc6f2c3e44c48dc517eb050ca29
GO